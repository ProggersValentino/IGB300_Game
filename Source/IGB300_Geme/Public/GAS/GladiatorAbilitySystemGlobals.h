// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FGladiatorGameplayEffectContext.h"
#include "GladiatorAbilitySystemGlobals.generated.h"

/**
 * The purpose of this class is to allow customisation around GAS classes if we so desire to customise them like
 * the GameplayEffectContext or GameplayCueManager
 *
 * This Subclass of UAbilitySystemGlobals is set in the DefaultGame.ini which essentially overrides the current AbilitySystemGlobals to this one allowing us to
 * set our customised subclasses to be default whenever executing a GAS system like the GameplayEffectContext
 */
UCLASS()
class IGB300_GEME_API UGladiatorAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

protected:
	//set the GameplayContext to our custom one made
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override
	{
		return new FGladiatorGameplayEffectContext();
	}
	
	
};
