// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/FGladiatorGameplayEffectContext.h"

FGladiatorGameplayEffectContext::FGladiatorGameplayEffectContext()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Gladiator Context Created"));
}

bool FGladiatorGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
