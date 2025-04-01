// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	playerPosQueue.SetNum(10); // Set default position buffer to 10
	enemyUidToAssign = 0;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	RegisterSpawns();

	player = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass());

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePlayerPosition();
}

void AEnemyManager::RegisterSpawns()
{
	// Save actors of the spawnLocation class
	TArray<AActor*> spawnLocationActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnLocation::StaticClass(), spawnLocationActors);

	for (int32 i = 0; i != spawnLocationActors.Num(); ++i) {

		// Add spawn locations to our hashmap
		spawnLocations.Add(i, Cast<ASpawnLocation>(spawnLocationActors[i])->RegisterSpawnLocation(i));

	}
}

void AEnemyManager::UpdatePlayerPosition() {
	for (int32 i = playerPosQueue.Num() - 1; i > 0; --i) {
		playerPosQueue.Swap(i, i - 1);
	}
	playerPosQueue[0] = player->GetActorLocation();
}

FVector& AEnemyManager::ReturnPlayerPos(int32 delay) {
	return playerPosQueue[delay];
}

// Register enemy into map and return uid to enemy that called it
int32 AEnemyManager::RegisterEnemy(AEnemyBase* enemy){
	enemies.Add(enemy);
	return enemyUidToAssign++;
}

void AEnemyManager::DeregisterEnemy(AEnemyBase* enemy){
	enemies.Remove(enemy);
}

void AEnemyManager::Spawn(int32 spawnIndex) {
	if (spawnLocations.Num() > 0) {
		// Set Spawn Location
		int32 spawnLocInd = 0;
		FVector location;
		FRotator rot(0.0f, 0.0f, 0.0f);

		
		int32 i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Basic);
		while (i > 0) {
			location = *spawnLocations.Find(spawnLocInd % spawnLocations.Num());
			GetWorld()->SpawnActor<AActor>(bpEnemyMelee, location , rot);
			spawnLocInd++;
			i--;
		}

		i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Tank);
		while (i > 0) {
			location = *spawnLocations.Find(spawnLocInd % spawnLocations.Num());
			GetWorld()->SpawnActor<AActor>(bpEnemyTank, location , rot);
			spawnLocInd++;
			i--;
		}

		i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Ranged);
		while (i > 0) {
			location = *spawnLocations.Find(spawnLocInd % spawnLocations.Num());
			GetWorld()->SpawnActor<AActor>(bpEnemyRanged, location , rot);
			spawnLocInd++;
			i--;
		}
	}
}



	FVector AEnemyManager::TargetEnemyPositionCalculator(FVector playerPos, FVector enemyPos, FVector ArenaPos, float zoneR, float arenaR) const{
		FVector ret = FVector(0.0f, 0.0f, 0.0f);

		// Assume ideal point is inside the arena (edge case is dealt with later)
		ret.X = playerPos.X +
		        (enemyPos.X-playerPos.X) *
		        zoneR /
						UKismetMathLibrary::Sqrt(
						  UKismetMathLibrary::Square(enemyPos.X-playerPos.X) +
						  UKismetMathLibrary::Square(enemyPos.Y-playerPos.Y));
	
		ret.Y = playerPos.Y +
		        (enemyPos.Y-playerPos.Y) *
						zoneR /
						UKismetMathLibrary::Sqrt(
							UKismetMathLibrary::Square(enemyPos.X-playerPos.X)+
							UKismetMathLibrary::Square(enemyPos.Y-playerPos.Y));

		// Check if ideal position is inside the arena
		float distToArenaCentre = UKismetMathLibrary::Square(ret.X - ArenaPos.X) +
		                          UKismetMathLibrary::Square(ret.Y - ArenaPos.Y);

		// Early return if ideal position is in arena
		if (distToArenaCentre < UKismetMathLibrary::Square(arenaR)){
			return ret;
		}

		// Get the quadratic sqrt
		float q = playerPos.Y *
		          UKismetMathLibrary::Sqrt(
		        		( 2 * UKismetMathLibrary::Square(arenaR) * UKismetMathLibrary::Square(playerPos.X) +
		        		2 * UKismetMathLibrary::Square(arenaR) * UKismetMathLibrary::Square(playerPos.Y) +
		        		2 * UKismetMathLibrary::Square(arenaR) * UKismetMathLibrary::Square(zoneR) +
		        		2 * UKismetMathLibrary::Square(zoneR) * UKismetMathLibrary::Square(playerPos.X) +
		        		2 * UKismetMathLibrary::Square(zoneR) * UKismetMathLibrary::Square(playerPos.Y) 
		        		) -
		        		(FMath::Pow(playerPos.X, 4) +
		        		2 * UKismetMathLibrary::Square(playerPos.X) * UKismetMathLibrary::Square(playerPos.Y) +
								FMath::Pow(playerPos.X, 4) +
		        		FMath::Pow(zoneR, 4) +
								FMath::Pow(arenaR, 4)
		        	  )
		        );
	
	// Get a->x where the quad is positive
	float a = (q +
	          FMath::Pow(playerPos.X, 3) +
	          playerPos.X * UKismetMathLibrary::Square(arenaR) +
	          playerPos.X * UKismetMathLibrary::Square(playerPos.Y) -
	          playerPos.X * UKismetMathLibrary::Square(zoneR)
	        )/ (2 * UKismetMathLibrary::Square(playerPos.Y) * UKismetMathLibrary::Square(playerPos.X));

	// Get b->x where quad is negative
	float b = (-q +
	          FMath::Pow(playerPos.X, 3) +
	          playerPos.X * UKismetMathLibrary::Square(arenaR) +
	          playerPos.X * UKismetMathLibrary::Square(playerPos.Y) -
	          playerPos.X * UKismetMathLibrary::Square(zoneR)
	        )/ (2 * UKismetMathLibrary::Square(playerPos.Y) * UKismetMathLibrary::Square(playerPos.X));

	// Four Returns one for each quadrant
	if (playerPos.X * playerPos.Y > 0){
		if (playerPos.X > 0){
			ret.X = b;
			ret.Y = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(arenaR) - UKismetMathLibrary::Square(b));
		} else { 
			ret.X = b;
			ret.Y = -UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(arenaR) - UKismetMathLibrary::Square(b));
		}
	} else {
		if (playerPos.X > 0) {
			ret.Y = a;
			ret.Y = -UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(arenaR) - UKismetMathLibrary::Square(a));
		}	else {
			ret.Y = a;
			ret.Y = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(arenaR) - UKismetMathLibrary::Square(a));
		}
	}
		return ret;
	}
