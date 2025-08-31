// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Difficulty.h"
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "CustomError.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "IGB300_Geme/BasicMeleeEnemy.h"
#include "IGB300_Geme/EnemyManager.h"
#include "IGB300_Geme/SpawnLocation.h"
#include "Math/MathFwd.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemySubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)

struct FSpawnResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bDidSucceed;

	UPROPERTY(BlueprintReadOnly)
	int32 AmountSpawned;

	UPROPERTY(BlueprintReadOnly)
	int32 RemainingPool;
};

struct FEnemyInfo
{
	AActor* Enemy;
	FVector TargetPos;
};

UCLASS()

class IGB300_GEME_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyPool = 100;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable)
	FSpawnResult TrySpawn(EDifficulty Difficulty, TSubclassOf<AEnemyBase> EnemyClass);

	UFUNCTION(BlueprintCallable)
	void ChangePool(EDifficulty Difficulty);

	UFUNCTION(BlueprintCallable)
	void DeregisterEnemy(AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	void UpdateTargetPositions();

	UFUNCTION(BlueprintCallable)
	FVector RequestTargetPosition(AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	FVector RequestPlayerPosition();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetEnemies();
	
private:
	TArray<ASpawnLocation*> SpawnLocations;

	TArray<FEnemyInfo> EnemyList;

	AActor* Player;

	float ZoneRadius = 300.0f;

	float ArenaRadius = 1500.0f;

	float EnemyBoidRepelThreshold = 200.0f;

	float EnemyBoidRepelStrength = 50.0f;

private:
	bool DoesLevelContainSpawns();

	FVector TargetEnemyPositionCalculator(FVector EnemyPosition);

	void RetargetEnemyPositionRepel(FEnemyInfo& Enemy);
};
