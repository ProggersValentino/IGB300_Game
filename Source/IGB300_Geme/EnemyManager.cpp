// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EnemyBase.h"


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

