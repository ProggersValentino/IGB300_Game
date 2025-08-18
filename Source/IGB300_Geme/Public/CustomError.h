// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomError.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCustomError
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
  bool Ok;
	UPROPERTY(BlueprintReadOnly)
  FString Error;
};
