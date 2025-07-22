// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "IGB300_Geme/SpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"
#include <algorithm>

void UEnemySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
  bool b_shouldCreate = HasRegisteredSpawns();
  if (!b_shouldCreate)
  {
    return;
  } 
}

// I hate this name TODO: refactor
bool UEnemySubsystem::HasRegisteredSpawns()
{
  TArray<AActor*> spawnLocations;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnLocation::StaticClass(), spawnLocations);
  if (spawnLocations.Num() == 0)
  {
    return false;
  }
  for (int32 i = 0; i < spawnLocations.Num(); i++)
  {
    SpawnLocations.Add(Cast<ASpawnLocation>(spawnLocations[i]));
  }
  return true;
}

FCustomError UEnemySubsystem::TrySpawn(float Difficulty, TSubclassOf<AEnemyBase> EnemyClass)
{
  Difficulty = std::min(Difficulty, (float) EnemyPool);
  if (Difficulty == 0)
  {
    return {false, "Enemy Pool Empty"};
  }
  for (int i = 0; i < Difficulty; i++)
  {
    FVector Position;
    FRotator Rotation;
    GetWorld()->SpawnActor<AActor>(EnemyClass, Position, Rotation);
  }
  EnemyPool -= Difficulty;
  return {true, ""};
}

// Change to spawn at location
// Look at enemy ai controller or put logic here
// Add enemy registration and deregistration
