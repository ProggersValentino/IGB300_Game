// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponAnimation.h"
#include "SWeaponData.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IGB300_GEME_API IIWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "IWeapon")
	FSWeaponData AddWeapon();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "IWeapon")
	bool DamageObject(AActor* enemy, float damage);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "IWeapon")
	bool BlockIncomingDamage();
	
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "IWeapon")
	UAnimMontage* RetrieveAnimation(EWeaponAnimation selectedAnim);
};
