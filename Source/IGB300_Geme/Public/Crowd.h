// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crowd.generated.h"

UCLASS()
class IGB300_GEME_API ACrowd : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrowd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions for Excitement
	UFUNCTION(BlueprintCallable)
	virtual void ResetExcitement();

	UFUNCTION(BlueprintCallable)
	virtual float UpdateExcitement(float XP);

	UFUNCTION(BlueprintCallable)
	virtual bool KeepCoinsActive();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetExcitement();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetMaxExcitement();

	//Excitement variables
	UPROPERTY(VisibleAnywhere, Category="Crowd")
	float Excitement;

	UPROPERTY(VisibleAnywhere, Category="Crowd")
	float CumulativeExcitement;

	UPROPERTY(EditAnywhere, Category="Crowd")
	float MaxExcitement;

	// Coin variables
	UPROPERTY(VisibleAnywhere, Category="Coin")
	float Aureus;

	UPROPERTY(EditAnywhere, Category="Coin")
	float TimeActive;
};
