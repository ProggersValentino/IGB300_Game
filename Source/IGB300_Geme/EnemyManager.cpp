// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	RegisterSpawns();

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

