// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifyState_GameplayEffect.h"

void UAnimNotifyState_GameplayEffect::Init(USkeletalMeshComponent* MeshComp)
{
	if (!IsValid(MeshComp)) return;
	
	character = Cast<AGladiatorBaseChar>(MeshComp->GetOwner());

	if (character) abilitySystem = Cast<UGladiatorAbilitySystemComponent>(character->GetAbilitySystemComponent());
	
}

void UAnimNotifyState_GameplayEffect::InitGameplayEffect(TSubclassOf<UGameplayEffect> effect)
{

	if (!IsValid(effect) || !IsValid(character))
	{
		return;
	}
	
	FGameplayEffectSpecHandle SpecHandle = abilitySystem->MakeOutgoingSpec(effect, 1.f, abilitySystem->MakeEffectContext());
	
	ActiveGameplayEffect = abilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); 
}

void UAnimNotifyState_GameplayEffect::RemoveEffect()
{
	if (!IsValid(character))
	{
		return;
	}
	
	abilitySystem->RemoveActiveGameplayEffect(ActiveGameplayEffect); //removes the current gameplay effect
}
