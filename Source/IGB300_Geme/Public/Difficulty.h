// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	EASY = 0 UMETA(DisplayName = "EASY"),
 	NORMAL  = 1 UMETA(DisplayName = "NORMAL"),
 	HARD    = 2 UMETA(DisplayName = "HARD"),
 	HARDEST = 3 UMETA(DisplayName = "HARDEST")
};
