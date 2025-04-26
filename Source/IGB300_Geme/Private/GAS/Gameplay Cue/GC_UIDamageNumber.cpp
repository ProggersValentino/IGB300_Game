// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Gameplay Cue/GC_UIDamageNumber.h"

#include "Blueprint/UserWidget.h"
#include "GAS/FGladiatorGameplayEffectContext.h"

bool AGC_UIDamageNumber::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	//get our custom Gameplay Context
	const FGladiatorGameplayEffectContext* context = static_cast<const FGladiatorGameplayEffectContext*>(Parameters.EffectContext.Get());

	if (!DamageNumberWidgetClass) return false;

	//Spawn damage number widget
	DamageNumberWidget = CreateWidget<UDamageNumberWidget_Base>(PlayerController, DamageNumberWidgetClass);

	UE_LOG(LogTemp, Warning, TEXT("Damage is: %f"), context->Data_Damage);

	//setting values within the widget
	DamageNumberWidget->Damage = context->Data_Damage; //Data_Damage gets set in the execution calculation
	DamageNumberWidget->Target = MyTarget;
	
	//adding it to the player's viewport so it shows up in game
	DamageNumberWidget->AddToViewport();
	
	return true;
	
}
