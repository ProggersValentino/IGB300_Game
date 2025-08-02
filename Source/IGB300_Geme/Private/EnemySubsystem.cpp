// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySubsystem.h"
#include "Difficulty.h"
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

FSpawnResult UEnemySubsystem::TrySpawn(EDifficulty Difficulty, TSubclassOf<AEnemyBase> EnemyClass)
{
  int SpawnAmount = 0;
  switch (Difficulty)
  {
    case EDifficulty::NORMAL:
    {
      SpawnAmount = 3;
    } break;

    case EDifficulty::HARD:
    {
      SpawnAmount = 5;
    } break;

    case EDifficulty::HARDEST:
    {
      SpawnAmount = 7;
    } break;
  }

  if (EnemyPool == 0)
  {
    return {false, 0, 0};
  }

  if (SpawnAmount > EnemyPool)
  {
    SpawnAmount = EnemyPool;
  }

  for (int i = 0; i < SpawnAmount; i++)
  {
    FVector Position;
    FRotator Rotation;
    GetWorld()->SpawnActor<AActor>(EnemyClass, Position, Rotation);
  }
  EnemyPool -= SpawnAmount;
  return {true, SpawnAmount, EnemyPool};
}

void UEnemySubsystem::ChangePool(EDifficulty Difficulty)
{
  switch (Difficulty)
  {
    case EDifficulty::NORMAL:
    {
      EnemyPool = 12;
    } return;

    case EDifficulty::HARD:
    {
      EnemyPool = 20;
    } return;

    case EDifficulty::HARDEST:
    {
      EnemyPool = 28;
    } return;
  }
}

// Change to spawn at location               | within try spawn
// Add enemy registration and deregistration | private register public deregister
// Setup enemy behaviour                     | public update enemy targets

