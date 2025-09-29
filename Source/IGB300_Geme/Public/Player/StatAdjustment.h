// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatAdjustment.generated.h"

/**
 * to carry over data for combat related requests to process and adjust the stats of the attacker/defender on
 * ability executed
 */
UCLASS()
class IGB300_GEME_API UStatAdjustment : public UObject
{
	GENERATED_BODY()
public:
	
	float damageAdjustment = 0.f;
};
