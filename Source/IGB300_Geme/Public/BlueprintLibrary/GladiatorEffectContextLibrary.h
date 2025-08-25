// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/FGladiatorGameplayEffectContext.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GladiatorEffectContextLibrary.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGladiatorEffectContextLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gladiator Effect Context")
	static FGladiatorGameplayEffectContext GetGladiatorGameplayEffectContext(FGameplayEffectContextHandle ContextHandle)
	{
		/*if we can successfully cast the context to our custom class return it dereferenced*/
		if(FGladiatorGameplayEffectContext* dereived = static_cast<FGladiatorGameplayEffectContext*>(ContextHandle.Get()))
		{
			return *dereived;
		}

		return FGladiatorGameplayEffectContext();
		
	}
	
};
