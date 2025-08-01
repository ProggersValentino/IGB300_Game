// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ComboContainer.h"

void UComboContainer::Init(UGladiatorAbilitySystemComponent* abilityComp)
{
	selectedCharacter = abilityComp;
}

void UComboContainer::ExecuteCombo()
{
	selectedCharacter->TryActivateAbilityByClass(ChosenAbility);
}
