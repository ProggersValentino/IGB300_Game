// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

UCLASS()

class IGB300_GEME_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyPool = 2;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable)
	FCustomError TrySpawn(float Difficulty, TSubclassOf<AEnemyBase> EnemyClass);
	
private:
	TArray<ASpawnLocation*> SpawnLocations;

	bool HasRegisteredSpawns();
};
