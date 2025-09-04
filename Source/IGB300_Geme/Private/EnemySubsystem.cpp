// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySubsystem.h"
#include "CoreGlobals.h"
#include "Difficulty.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "IGB300_Geme/SpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogMacros.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"
#include <algorithm>
#include <vcruntime_typeinfo.h>

void UEnemySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
  bool b_shouldCreate = DoesLevelContainSpawns();
	Player = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass());
  if (!b_shouldCreate)
  {
    return;
  } 
}

bool UEnemySubsystem::DoesLevelContainSpawns()
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

FVector GetRandomSpawnLocation(TArray<ASpawnLocation*>& SpawnLocations)
{
  int32 i = FMath::RandRange(0, SpawnLocations.Num() - 1); 
  return SpawnLocations[i]->GetActorLocation();
}

FSpawnResult UEnemySubsystem::TrySpawn(EDifficulty Difficulty, TSubclassOf<AEnemyBase> EnemyClass)
{
  int SpawnAmount = 0;
  switch (Difficulty)
  {
  case EDifficulty::EASY:
	  {
		  SpawnAmount = 1;
	  } break;
    case EDifficulty::NORMAL:
    {
      SpawnAmount = 3;
    } break;

    case EDifficulty::HARD:
    {
      SpawnAmount = 7;
    } break;

    case EDifficulty::HARDEST:
    {
      SpawnAmount = 15;
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
    FVector Position = GetRandomSpawnLocation(SpawnLocations);
    FRotator Rotation;
    AActor* Enemy = GetWorld()->SpawnActor<AActor>(EnemyClass, Position, Rotation);
    FEnemyInfo Info = {Enemy, Position};
    EnemyList.Add(Info);
  }
  EnemyPool -= SpawnAmount;
  return {true, SpawnAmount, EnemyPool};
}

void UEnemySubsystem::DeregisterEnemy(AActor* Enemy)
{
  for (int32 i = 0; i < EnemyList.Num(); i++)
  {
    if (Enemy == EnemyList[i].Enemy)
    {
      EnemyList.RemoveAt(i);
      return;
    }
  }
}

void UEnemySubsystem::ChangePool(EDifficulty Difficulty)
{
  switch (Difficulty)
  {
  	case EDifficulty::EASY:
  		{
  			EnemyPool = 10;
  		} return;
    case EDifficulty::NORMAL:
    {
      EnemyPool = 25;
    } return;

    case EDifficulty::HARD:
    {
      EnemyPool = 50;
    } return;

    case EDifficulty::HARDEST:
    {
      EnemyPool = 75;
    } return;
  }
}


void UEnemySubsystem::UpdateTargetPositions()
{
  for (FEnemyInfo &e : EnemyList)
  {
  	if (e.Enemy)
  	{
  		e.TargetPos = TargetEnemyPositionCalculator(e.Enemy->GetActorLocation());
  	}    
  }

  for (FEnemyInfo &e : EnemyList)
  {
  	if (e.Enemy)
  	{
  		RetargetEnemyPositionRepel(e);
  	}    
  }
}

FVector UEnemySubsystem::RequestTargetPosition(AActor* Enemy)
{
  for (FEnemyInfo e : EnemyList)
  {
    if (e.Enemy == Enemy)
    {
      return e.TargetPos;
    }
  }
  return {0, 0, 0}; // unreachable
}

FVector UEnemySubsystem::TargetEnemyPositionCalculator(FVector EnemyPos)
{
  FVector PlayerPos = Player->GetActorLocation();
  FVector ArenaPos = FVector(0.0f, 0.0f, 0.0f);
  typedef UKismetMathLibrary UKML;
	FVector ret = FVector(0.0f, 0.0f, 0.0f);

	// Assume ideal point is inside the arena (edge case is dealt with later)
	ret.X = PlayerPos.X
	  + (EnemyPos.X-PlayerPos.X)
	  * ZoneRadius
	  / UKML::Sqrt(
		  UKML::Square(EnemyPos.X-PlayerPos.X) +
		  UKML::Square(EnemyPos.Y-PlayerPos.Y)
	  )
  ;
	
	ret.Y = PlayerPos.Y
	  + (EnemyPos.Y-PlayerPos.Y)
	  *	ZoneRadius
	  /	UKML::Sqrt(
	  	UKML::Square(EnemyPos.X-PlayerPos.X)+
			UKML::Square(EnemyPos.Y-PlayerPos.Y)
		)
  ;

	// Check if ideal position is inside the arena
	float distToArenaCentre = UKML::Square(ret.X - ArenaPos.X) + UKML::Square(ret.Y - ArenaPos.Y);

	// Early return if ideal position is in arena
	if (distToArenaCentre < UKML::Square(ArenaRadius))
	{
		return ret;
	}

	// Get the quadratic sqrt
	float q = PlayerPos.Y
	  * UKML::Sqrt(
			( 2 * UKML::Square(ArenaRadius) * UKML::Square(PlayerPos.X) +
		 		2 * UKML::Square(ArenaRadius) * UKML::Square(PlayerPos.Y) +
		 		2 * UKML::Square(ArenaRadius) * UKML::Square(ZoneRadius) +
		 		2 * UKML::Square(ZoneRadius) * UKML::Square(PlayerPos.X) +
		 		2 * UKML::Square(ZoneRadius) * UKML::Square(PlayerPos.Y) 
		  )
		  -
  		( FMath::Pow(PlayerPos.X, 4)
  		+	2 * UKML::Square(PlayerPos.X) * UKML::Square(PlayerPos.Y)
  		+ FMath::Pow(PlayerPos.Y, 4)
  		+ FMath::Pow(ZoneRadius, 4)
  		+	FMath::Pow(ArenaRadius, 4)
  		)
	 )
  ;
	
	// Get a->x where the quad is positive
	float a = (q + FMath::Pow(PlayerPos.X, 3) + PlayerPos.X * UKML::Square(ArenaRadius) + PlayerPos.X * UKML::Square(PlayerPos.Y) - PlayerPos.X * UKML::Square(ZoneRadius))
	  / (2 * (UKML::Square(PlayerPos.Y) + UKML::Square(PlayerPos.X)))
	;

	// Get b->x where quad is negative
	float b = (-q + FMath::Pow(PlayerPos.X, 3) + PlayerPos.X * UKML::Square(ArenaRadius) + PlayerPos.X * UKML::Square(PlayerPos.Y) - PlayerPos.X * UKML::Square(ZoneRadius))
	  / (2 * (UKML::Square(PlayerPos.Y) + UKML::Square(PlayerPos.X)))
	;

	// Four Returns one for each quadrant
	if (PlayerPos.X * PlayerPos.Y > 0)
	{
		if (PlayerPos.X > 0)
		{
			ret.X = b;
			ret.Y = UKML::Sqrt(UKML::Square(ArenaRadius) - UKML::Square(b));
		}
		else
		{ 
			ret.X = b;
			ret.Y = -UKML::Sqrt(UKML::Square(ArenaRadius) - UKML::Square(b));
		}
	}
	else
	{
		if (PlayerPos.X > 0) {
			ret.X = a;
			ret.Y = -UKML::Sqrt(UKML::Square(ArenaRadius) - UKML::Square(a));
		}	else {
			ret.X = a;
			ret.Y = UKML::Sqrt(UKML::Square(ArenaRadius) - UKML::Square(a));
		}
	}
	return ret;
}

void UEnemySubsystem::RetargetEnemyPositionRepel(FEnemyInfo& Enemy)
{
  for (FEnemyInfo e : EnemyList)
  {
    if (e.Enemy == Enemy.Enemy)
    {
      return;
    }

    if (FVector::Dist(e.TargetPos, Enemy.TargetPos) < EnemyBoidRepelThreshold)
    {
      FVector Distance = Enemy.TargetPos - e.TargetPos;
      Distance.Normalize();
      Enemy.TargetPos = (Distance * EnemyBoidRepelStrength) + Enemy.TargetPos;
    }
  }  
}

FVector UEnemySubsystem::RequestPlayerPosition()
{
  return Player->GetActorLocation();
}
// Setup enemy behaviour                     | public update enemy targets
// Check for spawns rename

int UEnemySubsystem::GetEnemyCount()
{
	return EnemyList.Num();
}

void UEnemySubsystem::StartFearing()
{
  ZoneRadius = 1000.0f;
  b_IsFearing = true;
  return;
}
void UEnemySubsystem::EndFearing()
{
  ZoneRadius = 300.0f;
  b_IsFearing = false;
  return;
}

