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
	FVector OldTargetPos;
	FVector TargetPos;
	bool isAggresive;
	float ZoneRadius;
};

UCLASS()

class IGB300_GEME_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyPool = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool b_IsFearing = false;

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
	int GetEnemyCount();

	UFUNCTION(BlueprintCallable)
	void StartFearing();

	UFUNCTION(BlueprintCallable)
	void EndFearing();
	

	UFUNCTION(BlueprintCallable)
	void SetAggressionAmountRandom();

	UFUNCTION(BlueprintCallable)
	void SetAggressionAmount(int amount);

	private:
	TArray<ASpawnLocation*> SpawnLocations;

	TArray<FEnemyInfo> EnemyList;

	AActor* Player;

	float ZoneRadius = 500.0f;
	float ZoneRadiusMin = 300.0f;
	float ZoneRadiusMax = 450.0f;

	float ArenaRadius = 1500.0f;

	float EnemyBoidRepelThreshold = 100.0f;

	float EnemyBoidRepelStrength = 500.0f;

	int EnemyAgressionCount = 2;
	int EnemyAggressionMin = 1;	
	int EnemyAggressionMax = 4;

private:
	bool DoesLevelContainSpawns();

	FVector TargetEnemyPositionCalculator(FVector EnemyPosition, float radius);

	void RetargetEnemyPositionRepel(FEnemyInfo& Enemy);

	void SetAggression();
};
