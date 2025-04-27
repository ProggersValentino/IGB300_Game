// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "FGladiatorGameplayEffectContext.generated.h"



///
///The purpose of this script is to provide a way to parse data from execution calculations or other scripts that are not accessible through the traditional way
///This is the applied to the UGladiatorAbilitySystemGlobals which handles spawning in the instances of the GAS related systems overriding the default FGameplayEffectContext

USTRUCT(Blueprintable)
struct FGladiatorGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FGladiatorGameplayEffectContext();

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Data_Damage = 0.0f; 
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGladiatorGameplayEffectContext::StaticStruct();
	}

	virtual FGameplayEffectContext* Duplicate() const override
	{
		return new FGladiatorGameplayEffectContext(*this);
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
};

template<>
struct TStructOpsTypeTraits<FGladiatorGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGladiatorGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};



