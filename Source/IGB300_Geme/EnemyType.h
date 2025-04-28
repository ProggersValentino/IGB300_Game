// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BluePrintType)
enum class EEnemyType : uint8
{
	ET_Basic	UMETA(DisplayName = "Basic"),
	ET_Ranged	UMETA(DisplayName = "Ranged"),
	ET_Tank		UMETA(DisplayName = "Tank")
};