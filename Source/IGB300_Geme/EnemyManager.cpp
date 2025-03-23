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
	enemies.Add(enemyUidToAssign++, enemy);
	return enemyUidToAssign - 1;
}

void AEnemyManager::DeregisterEnemy(int32 uid){
	enemies.Remove(uid);
}

void AEnemyManager::Spawn(int32 spawnIndex) {
	if (spawnLocations.Num() > 0) {
		// Set Spawn Location
		FVector loc(0.0f, 0.0f, 0.0f);
		FRotator rot(0.0f, 0.0f, 0.0f);

		int32 i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Basic);
		while (i > 0) {
			GetWorld()->SpawnActor<AActor>(bpEnemyMelee, loc, rot);
			i--;
		}

		i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Tank);
		while (i > 0) {
			GetWorld()->SpawnActor<AActor>(bpEnemyTank, loc, rot);
			i--;
		}

		i = *waves[spawnIndex].amounts.Find(EEnemyType::ET_Ranged);
		while (i > 0) {
			GetWorld()->SpawnActor<AActor>(bpEnemyRanged, loc, rot);
			i--;
		}
	}
}

