// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerState.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"

UAbilitySystemComponent* AGladiatorPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGladiatorAttributeSet* AGladiatorPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

AGladiatorPlayerState::AGladiatorPlayerState()
{
	NetUpdateFrequency = 100.f; //default value to low and may cause lag thus we bump up update frequency to 100 per second

	//init attribute set
	AttributeSet = CreateDefaultSubobject<UGladiatorAttributeSet>("AttributeSet");
	
	//creating ability system comp and setting it to be explicity replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}
