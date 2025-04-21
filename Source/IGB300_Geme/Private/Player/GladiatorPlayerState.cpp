// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerState.h"

#include "Blueprint/UserWidget.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"
#include "Player/GladiatorPlayerChar.h"
#include "UI/GladiatorHUDBase.h"

UAbilitySystemComponent* AGladiatorPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGladiatorAttributeSet* AGladiatorPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

float AGladiatorPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

void AGladiatorPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	//binding delegates to the ASC so that when a attribute changes it sends a signal here to trigger the event
	HealthChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGladiatorPlayerState::HealthChanged);
}

void AGladiatorPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	AGladiatorPlayerChar* Player = Cast<AGladiatorPlayerChar>(GetPawn()); //getting the player

	if (!Player) return; //prevents Die function from being called post death of player (when it gets deleted) which will most certainly cause UE to crash 
	
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		Player->Die();
		//get HUD to display restart
		AGladiatorHUDBase* HUD = Cast<AGladiatorHUDBase>(GetPlayerController()->GetHUD());

		check(HUD);

		//spawn Death UI
		HUD->Death();
	}
}

bool AGladiatorPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

AGladiatorPlayerState::AGladiatorPlayerState()
{
	NetUpdateFrequency = 100.f; //default value to low and may cause lag thus we bump up update frequency to 100 per second

	//init attribute set
	AttributeSet = CreateDefaultSubobject<UGladiatorAttributeSet>("AttributeSet");
	
	//creating ability system comp and setting it to be explicity replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	DeathTag = FGameplayTag::RequestGameplayTag("Gameplay.State.Death");
}


