// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySubsystem.h"
#include "AI/Navigation/NavigationTypes.h"
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
#include "UObject/ObjectVersion.h"
#include <algorithm>
#include <vcruntime_typeinfo.h>
#include "Kismet/KismetArrayLibrary.h"
#include "Player/GladiatorPlayerChar.h"

void UEnemySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
  bool b_shouldCreate = DoesLevelContainSpawns();
	Player = UGameplayStatics::GetActorOfClass(GetWorld(), AGladiatorPlayerChar::StaticClass());
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

FSpawnResult UEnemySubsystem::TrySpawn(EDifficulty Difficulty, TArray<TSubclassOf<AEnemyBase>> EnemyClass)
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
      SpawnAmount = 2;
    } break;

    case EDifficulty::HARD:
    {
      SpawnAmount = 3;
    } break;

    case EDifficulty::HARDEST:
    {
      SpawnAmount = 4;
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
    int ClassIndex = FMath::RandRange(0, EnemyClass.Num()-1);
    AActor* Enemy = GetWorld()->SpawnActor<AActor>(EnemyClass[ClassIndex], Position, Rotation);
    float ZR = FMath::RandRange(ZoneRadiusMin, ZoneRadiusMax);
    FEnemyInfo Info = {Enemy, Position, Position, false, ZR};
    EnemyList.Add(Info);
  }
  EnemyPool -= SpawnAmount;
  SetAggression();
  return {true, SpawnAmount, EnemyPool};
}

void UEnemySubsystem::DeregisterEnemy(AActor* Enemy)
{
  for (int32 i = 0; i < EnemyList.Num(); i++)
  {
    if (Enemy == EnemyList[i].Enemy)
    {
      EnemyList.RemoveAt(i);
      SetAggression();
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
  			EnemyPool = 100;
  		} return;
    case EDifficulty::NORMAL:
    {
      EnemyPool = 250;
    } return;

    case EDifficulty::HARD:
    {
      EnemyPool = 500;
    } return;

    case EDifficulty::HARDEST:
    {
      EnemyPool = 750;
    } return;
  }
}


void UEnemySubsystem::UpdateTargetPositions()
{
  for (FEnemyInfo &e : EnemyList)
  {
  	if (e.Enemy)
  	{
  	  e.OldTargetPos = e.TargetPos;
  	  if (e.isAggresive) {
    		e.TargetPos = TargetEnemyPositionCalculator(e.Enemy->GetActorLocation(), 150.0f);
  	  } else {
    		e.TargetPos = TargetEnemyPositionCalculator(e.Enemy->GetActorLocation(), e.ZoneRadius);
  	  }
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

FVector UEnemySubsystem::TargetEnemyPositionCalculator(FVector EnemyPos, float radius)
{
  FVector PlayerPos = Player->GetActorLocation();
  FVector ArenaPos = FVector(0.0f, 0.0f, 0.0f);
  typedef UKismetMathLibrary UKML;
	FVector ret = FVector(0.0f, 0.0f, 0.0f);

	// Assume ideal point is inside the arena (edge case is dealt with later)
	ret.X = PlayerPos.X
	  + (EnemyPos.X-PlayerPos.X)
	  * radius
	  / UKML::Sqrt(
		  UKML::Square(EnemyPos.X-PlayerPos.X) +
		  UKML::Square(EnemyPos.Y-PlayerPos.Y)
	  )
  ;
	
	ret.Y = PlayerPos.Y
	  + (EnemyPos.Y-PlayerPos.Y)
	  *	radius
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
	float first_part = 
			( 2 * UKML::Square(ArenaRadius) * UKML::Square(PlayerPos.X) +
		 		2 * UKML::Square(ArenaRadius) * UKML::Square(PlayerPos.Y) +
		 		2 * UKML::Square(ArenaRadius) * UKML::Square(radius) +
		 		2 * UKML::Square(radius) * UKML::Square(PlayerPos.X) +
		 		2 * UKML::Square(radius) * UKML::Square(PlayerPos.Y) 
		  );

	float second_part = 
  		( FMath::Pow(PlayerPos.X, 4)
  		+	2 * UKML::Square(PlayerPos.X) * UKML::Square(PlayerPos.Y)
  		+ FMath::Pow(PlayerPos.Y, 4)
  		+ FMath::Pow(radius, 4)
  		+	FMath::Pow(ArenaRadius, 4)
  		);

	float q;
	if (first_part > second_part) {
	  q = PlayerPos.Y * UKML::Sqrt(first_part - second_part);
	} else {
	  q = 0;
	}

	// Get a->x where the quad is positive
	float a = (q + FMath::Pow(PlayerPos.X, 3) + PlayerPos.X * UKML::Square(ArenaRadius) + PlayerPos.X * UKML::Square(PlayerPos.Y) - PlayerPos.X * UKML::Square(radius))
	  / (2 * (UKML::Square(PlayerPos.Y) + UKML::Square(PlayerPos.X)))
	;

	// Get b->x where quad is negative
	float b = (-q + FMath::Pow(PlayerPos.X, 3) + PlayerPos.X * UKML::Square(ArenaRadius) + PlayerPos.X * UKML::Square(PlayerPos.Y) - PlayerPos.X * UKML::Square(radius))
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

    e.TargetPos = e.OldTargetPos + 1.0f * (e.TargetPos - e.OldTargetPos).Normalize();
  }  
}

FVector UEnemySubsystem::RequestPlayerPosition()
{
  return Player->GetActorLocation();
}

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
  ZoneRadius = 500.0f;
  b_IsFearing = false;
  return;
}

void UEnemySubsystem::SetAggression()
{
  int numEnemies = EnemyList.Num();
  // Set all enemies aggressive
  if (numEnemies < EnemyAgressionCount + 1) {
    for (FEnemyInfo& e : EnemyList) {
      e.isAggresive = true;
    }
    return;
  }

  // Reset Aggression
  for (FEnemyInfo& e : EnemyList) {
    e.isAggresive = false;
  }
  
  // Set First two as aggressive
  for (int i = 0; i < EnemyAgressionCount; i++) {
    EnemyList[i].isAggresive = true;
  }
  return;
}

void UEnemySubsystem::SetAggressionAmountRandom() {
  EnemyAgressionCount = FMath::RandRange(EnemyAggressionMin, EnemyAggressionMax);
  SetAggression();
}

void UEnemySubsystem::SetAggressionAmount(int amount) {
  EnemyAgressionCount = amount;
  SetAggression();
}
