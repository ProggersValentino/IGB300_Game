// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IGB300_GEME_API IIEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Attack();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
		void Move();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Damage(float amount);
};
