// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBaseChar.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"
#include "Player/GladiatorPlayerState.h"


UAbilitySystemComponent* AGladiatorBaseChar::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Sets default values
AGladiatorBaseChar::AGladiatorBaseChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
}

UGladiatorAttributeSet* AGladiatorBaseChar::GetAttributeSet() const
{
	return AttributeSet;
}

#pragma region AGladiatorAbilitySystemComponent Getters

float AGladiatorBaseChar::GetHealth() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->Health.GetCurrentValue();
}

float AGladiatorBaseChar::GetMaxHealth() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetMaxHealth();
}

float AGladiatorBaseChar::GetSpeed() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetSpeed();
}

float AGladiatorBaseChar::GetMaxSpeed() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetMaxSpeed();
}

float AGladiatorBaseChar::GetBaseDamage() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetBaseDamage();
}

float AGladiatorBaseChar::GetMaxBaseDamage() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetMaxBaseDamage();
}

float AGladiatorBaseChar::GetToughness() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetToughness();
}

float AGladiatorBaseChar::GetMaxToughness() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetMaxToughness();
}

float AGladiatorBaseChar::GetGold() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetGold();
}

float AGladiatorBaseChar::GetMaxGold() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetMaxGold();
}

bool AGladiatorBaseChar::activateAbilitiesWithTag(FGameplayTagContainer abilityTag, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent) return false;

	return AbilitySystemComponent->TryActivateAbilitiesByTag(abilityTag, AllowRemoteActivation);
}


#pragma endregion


// Called when the game starts or when spawned
void AGladiatorBaseChar::BeginPlay()
{
	Super::BeginPlay();
}

void AGladiatorBaseChar::GiveDefaultAbilities()
{
	check(AbilitySystemComponent);

	if (!HasAuthority()) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1); //data surrounding for the ability class

		//give ability to player
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AGladiatorBaseChar::InitDefaultAttributes() const
{
	if (!AbilitySystemComponent || !DefaultAttributeEffects) return;

	//applying default attribute effects to the characters ability system component
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext(); 
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, 1.f, EffectContext);

	//apply effect spec to character
	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());

		/*FString Message = FString::Printf(TEXT("new handle was valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, Message);*/

	}
	else
	{
		FString Message = FString::Printf(TEXT("new handle was not valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}
}

// Called every frame
void AGladiatorBaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGladiatorBaseChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




