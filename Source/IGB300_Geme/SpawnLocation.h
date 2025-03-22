// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation.generated.h"

UCLASS()
class IGB300_GEME_API ASpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnLocation();

	UPROPERTY(VisibleAnywhere, Category = "Members")
	int32 UID{ 0 };

	UPROPERTY(VisibleAnywhere, Category = "Members")
	FVector spawnPos;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	FVector RegisterSpawnLocation(int32 id);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
