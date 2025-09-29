// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaestroBase.generated.h"

UCLASS()
class IGB300_GEME_API AMaestroBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaestroBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Configuation")
	int CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Wave Configuation")
	int NumWaves = 2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetCurrentWave();

	int GetNumWaves();

	UFUNCTION(BlueprintCallable, Category = "Wave Configuation")
	void SetNumWaves(int amount);

	UFUNCTION(BlueprintImplementableEvent)
	void AchieveRageBaiter();
};
