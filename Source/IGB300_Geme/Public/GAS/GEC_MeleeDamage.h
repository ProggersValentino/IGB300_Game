// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_MeleeDamage.generated.h"

/**
 * The purpose of this class is to manage and output complex damage calculations that take in multiple attributes and applys the final calcualtion to the target the
 * effect is going for
 */
UCLASS()
class IGB300_GEME_API UGEC_MeleeDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEC_MeleeDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
