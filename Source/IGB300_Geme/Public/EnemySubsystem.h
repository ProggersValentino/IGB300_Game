// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Difficulty.h"
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "CustomError.h"
#include "IGB300_Geme/BasicMeleeEnemy.h"
#include "IGB300_Geme/EnemyManager.h"
#include "IGB300_Geme/SpawnLocation.h"
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

UCLASS()

class IGB300_GEME_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyPool = 20;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable)
	FSpawnResult TrySpawn(EDifficulty Difficulty, TSubclassOf<AEnemyBase> EnemyClass);

	UFUNCTION(BlueprintCallable)
	void ChangePool(EDifficulty Difficulty);
	
private:
	TArray<ASpawnLocation*> SpawnLocations;

	bool HasRegisteredSpawns();
};
