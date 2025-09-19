// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BlockedRageAttributeWidget.h"
#include "UI/GladiatorAttributeWidget.h"
#include "GAS/GladiatorAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GladiatorPlayerState.h"

void UBlockedRageAttributeWidget::BindAttributes()
{
	const AGladiatorPlayerState* GladiatorPlayerState = Cast<AGladiatorPlayerState>(GetOwningPlayerState());

	if (!GladiatorPlayerState) return;

	UAbilitySystemComponent* ASC = GladiatorPlayerState->GetAbilitySystemComponent();

	const UGladiatorAttributeSet* GladiatorAS = GladiatorPlayerState->GetAttributeSet();

	if (!GladiatorAS)
	{
		return;
	}

	BuiltBlockedRagePercent = GladiatorAS->GetBlockedRageAttribute().GetNumericValue(GladiatorAS) /
		GladiatorAS->GetMaxBlockedRageAttribute().GetNumericValue(GladiatorAS);

	//attribute changes -> happens everytime the attribute changes -> updates the desired attribute
	ASC->GetGameplayAttributeValueChangeDelegate(GladiatorAS->GetBlockedRageAttribute()).AddLambda(
	[this, GladiatorAS](const FOnAttributeChangeData& Data)
	{
		BuiltBlockedRagePercent = Data.NewValue / GladiatorAS->GetMaxBlockedRageAttribute().GetNumericValue(GladiatorAS);
	});

	
}
