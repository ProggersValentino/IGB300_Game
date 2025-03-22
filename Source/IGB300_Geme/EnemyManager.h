// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnLocation.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

class AEnemyBase;

UCLASS()
class IGB300_GEME_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyManager();

	UPROPERTY(VisibleAnywhere, Category = "Members")
		TMap<int32, FVector> spawnLocations;
	
	UPROPERTY(VisibleAnywhere, Category = "Members")
		TMap<int32, AEnemyBase*> enemies;

	AActor* player;

	UPROPERTY(VisibleAnywhere, Category = "Members")
		TArray<FVector> playerPosQueue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void RegisterSpawns();
	virtual void UpdatePlayerPosition();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector& ReturnPlayerPos(int32 delay);

};
