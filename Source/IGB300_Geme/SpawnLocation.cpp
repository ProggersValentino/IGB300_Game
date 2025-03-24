// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnLocation.h"

// Sets default values
ASpawnLocation::ASpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FVector ASpawnLocation::RegisterSpawnLocation(int32 id) {
	spawnPos = GetActorLocation();
	UID = id;
	return spawnPos;
}


