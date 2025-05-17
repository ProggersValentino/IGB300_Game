// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyType.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnLocation.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

class AEnemyBase;

USTRUCT(BlueprintType)
	struct FEnemyWave {
		GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TMap<EEnemyType, int32> amounts;
	};

UCLASS()
class IGB300_GEME_API AEnemyManager : public AActor
{
	GENERATED_BODY()

protected:
	int32 enemyUidToAssign;

public:
	// Sets default values for this actor's properties
	AEnemyManager();

	UPROPERTY(BluePrintReadOnly, Category = "Members")
		TArray<FEnemyWave> waves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Members")
		TMap<int32, FVector> spawnLocations;
	
	UPROPERTY(BlueprintReadWrite, Category = "Members")
		TArray<AEnemyBase*> enemies;

	AActor* player;

	UPROPERTY(VisibleAnywhere, Category = "Members")
		TArray<FVector> playerPosQueue;

	UPROPERTY(EditAnywhere, Category = "Enemy Blueprint References")
	TSubclassOf<AEnemyBase> bpEnemyMelee;
	UPROPERTY(EditAnywhere, Category = "Enemy Blueprint References")
	TSubclassOf<AEnemyBase> bpEnemyRanged;
	UPROPERTY(EditAnywhere, Category = "Enemy Blueprint References")
	TSubclassOf<AEnemyBase> bpEnemyTank;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void RegisterSpawns();
	virtual void UpdatePlayerPosition();


	UFUNCTION(BlueprintCallable)
	void Spawn(int32 waveIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector& ReturnPlayerPos(int32 delay);
	int32 RegisterEnemy(AEnemyBase*);
	void DeregisterEnemy(AEnemyBase*);
	UFUNCTION(BlueprintPure)
	FVector TargetEnemyPositionCalculator(FVector playerPos, FVector enemyPos, FVector ArenaPos, float zoneR, float arenaR) const;

};
