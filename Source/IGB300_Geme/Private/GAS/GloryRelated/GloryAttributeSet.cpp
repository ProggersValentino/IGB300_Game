// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GloryRelated/GloryAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGloryAttributeSet::UGloryAttributeSet()
{
}

void UGloryAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGloryAttributeSet, Glory, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UGloryAttributeSet, MaxGlory, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UGloryAttributeSet, m_Glory, COND_None, REPNOTIFY_Always)
}

void UGloryAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == Getm_GloryAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxGlory());
	}
}

void UGloryAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetGloryAttribute())
	{
		SetGlory(FMath::Clamp(GetGlory(), 0.0f, GetMaxGlory()));
	}
	
	if (Data.EvaluatedData.Attribute == Getm_GloryAttribute())
	{
		float newGloryValue = GetGlory() + Getm_Glory();
		Setm_Glory(0.0f);

		SetGlory(newGloryValue);
	}
}

void UGloryAttributeSet::OnRep_Glory(const FGameplayAttributeData& OldGlory) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGloryAttributeSet, Glory, OldGlory);
}

void UGloryAttributeSet::OnRep_MaxGlory(const FGameplayAttributeData& OldMaxGlory) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGloryAttributeSet, MaxGlory, OldMaxGlory);
}

void UGloryAttributeSet::OnRep_mGlory(const FGameplayAttributeData& OldMGlory) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGloryAttributeSet, m_Glory, OldMGlory);
}

