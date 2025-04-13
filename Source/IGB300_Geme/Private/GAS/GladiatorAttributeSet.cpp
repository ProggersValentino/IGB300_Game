// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GladiatorAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGladiatorAttributeSet::UGladiatorAttributeSet()
{
}

void UGladiatorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGladiatorAttributeSet, Health);
	DOREPLIFETIME(UGladiatorAttributeSet, MaxHealth);
	DOREPLIFETIME(UGladiatorAttributeSet, Speed);
	DOREPLIFETIME(UGladiatorAttributeSet, MaxSpeed);
	DOREPLIFETIME(UGladiatorAttributeSet, BaseDamage);
	DOREPLIFETIME(UGladiatorAttributeSet, MaxBaseDamage);
	DOREPLIFETIME(UGladiatorAttributeSet, Toughness);
	DOREPLIFETIME(UGladiatorAttributeSet, MaxToughness);
	DOREPLIFETIME(UGladiatorAttributeSet, Gold);
	DOREPLIFETIME(UGladiatorAttributeSet, MaxGold);

}

void UGladiatorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGladiatorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGladiatorAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Health, OldHealth);
}

void UGladiatorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxHealth, OldMaxHealth);
}

void UGladiatorAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Speed, OldSpeed);
}

void UGladiatorAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxSpeed, OldMaxSpeed);
}

void UGladiatorAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, BaseDamage, OldBaseDamage);
}

void UGladiatorAttributeSet::OnRep_MaxBaseDamage(const FGameplayAttributeData& OldMaxBaseDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxBaseDamage, OldMaxBaseDamage);
}

void UGladiatorAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Toughness, OldToughness);
}

void UGladiatorAttributeSet::OnRep_MaxToughness(const FGameplayAttributeData& OldMaxToughness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxToughness, OldMaxToughness);
}

void UGladiatorAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Gold, OldHealth);
}

void UGladiatorAttributeSet::OnRep_MaxGold(const FGameplayAttributeData& OldMaxGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxGold, OldMaxGold);
}
