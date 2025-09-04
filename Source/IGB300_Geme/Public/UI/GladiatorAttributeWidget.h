// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GladiatorAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGladiatorAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAttributes();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float HealthPercent; //how much health we have left

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float oldPercent;

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateAfterEffectHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateHealthEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHeartBeat(int healthState);

	int FindHealthState(float currentPercent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float timeTillActivation = 2.f;
	
private:
	FTimerHandle TimerHandle;
	
};
