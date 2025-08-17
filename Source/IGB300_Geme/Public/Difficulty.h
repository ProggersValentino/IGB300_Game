// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
 	NORMAL  = 0 UMETA(DisplayName = "NORMAL"),
 	HARD    = 1 UMETA(DisplayName = "HARD"),
 	HARDEST = 2 UMETA(DisplayName = "HARDEST")
};
