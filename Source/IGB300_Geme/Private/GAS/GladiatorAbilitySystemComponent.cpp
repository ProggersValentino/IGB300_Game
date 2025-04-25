// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GladiatorAbilitySystemComponent.h"

UGladiatorAbilitySystemComponent::UGladiatorAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}

void UGladiatorAbilitySystemComponent::ReiceveDamage(UGladiatorAbilitySystemComponent* SourceASC,
	float UnmitigatedDamage, float MitigatedDamage)
{
	RecieveDamage.Broadcast(SourceASC,UnmitigatedDamage,MitigatedDamage);
}
