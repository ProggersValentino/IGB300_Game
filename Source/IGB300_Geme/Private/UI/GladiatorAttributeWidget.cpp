// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GladiatorAttributeWidget.h"
#include "GAS/GladiatorAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GladiatorPlayerState.h"
#include "Slate/SGameLayerManager.h"

void UGladiatorAttributeWidget::BindToAttributes()
{
	const AGladiatorPlayerState* GladiatorPlayerState = Cast<AGladiatorPlayerState>(GetOwningPlayerState());

	if (!GladiatorPlayerState) return;

	UAbilitySystemComponent* ASC = GladiatorPlayerState->GetAbilitySystemComponent();

	const UGladiatorAttributeSet* GladiatorAS = GladiatorPlayerState->GetAttributeSet();

	//initial attributes
	HealthPercent = GladiatorAS->GetHealthAttribute().GetNumericValue(GladiatorAS) /
		GladiatorAS->GetMaxHealthAttribute().GetNumericValue(GladiatorAS);

	oldPercent = HealthPercent; //initial set
	
	//attribute changes -> happens everytime the attribute changes -> updates the desired attribute
	ASC->GetGameplayAttributeValueChangeDelegate(GladiatorAS->GetHealthAttribute()).AddLambda(
	[this, GladiatorAS](const FOnAttributeChangeData& Data)
	{
		
		HealthPercent = Data.NewValue / GladiatorAS->GetMaxHealthAttribute().GetNumericValue(GladiatorAS);

		ActivateHealthEffect();
		
		//engage "after health bar effect"  
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle); //if any current timers are going stop them cancelling the execution of the delegate
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGladiatorAttributeWidget::ActivateAfterEffectHealth, timeTillActivation, false);
	});
}
