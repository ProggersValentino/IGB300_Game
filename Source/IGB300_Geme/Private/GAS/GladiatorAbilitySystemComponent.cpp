// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GladiatorAbilitySystemComponent.h"

UGladiatorAbilitySystemComponent::UGladiatorAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastObjectThatHitData = nullptr;
	
	SetIsReplicated(true);
}

void UGladiatorAbilitySystemComponent::ReiceveDamage(UGladiatorAbilitySystemComponent* SourceASC,
	float UnmitigatedDamage, float MitigatedDamage)
{
	RecieveDamage.Broadcast(SourceASC,UnmitigatedDamage,MitigatedDamage);
}

const FGameplayEffectContextHandle UGladiatorAbilitySystemComponent::GetLastHitData() const
{
	return LastObjectThatHitData;
}

void UGladiatorAbilitySystemComponent::SetLastHitData(const FGameplayEffectContextHandle Context)
{
	LastObjectThatHitData = Context;
}

FGameplayEventData UGladiatorAbilitySystemComponent::MakeLastHitEventData() const
{
	FGameplayEventData lastHitData;
	
	FGameplayEffectContextHandle handle = LastObjectThatHitData;

	FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(*LastObjectThatHitData.Get()->GetHitResult());
	
	lastHitData.Instigator = LastObjectThatHitData.Get()->GetInstigator();
	lastHitData.ContextHandle = handle;
	lastHitData.TargetData = targetData;

	return lastHitData;
	
}
