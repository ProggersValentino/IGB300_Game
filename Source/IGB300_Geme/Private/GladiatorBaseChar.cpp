// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBaseChar.h"

#include "GAS/GladiatorAbilitySystemComponent.h"

UAbilitySystemComponent* AGladiatorBaseChar::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Sets default values
AGladiatorBaseChar::AGladiatorBaseChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating ability system comp and setting it to be explicity replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//init attribute set
	AttributeSet = CreateDefaultSubobject<UGladiatorAttributeSet>("AttributeSet");

	/*AttributeSet->Health.SetBaseValue(100.0f);
	AttributeSet->Health.SetCurrentValue(100.0f);*/
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), GetHealth())
	
}

#pragma region AGladiatorAbilitySystemComponent Getters

float AGladiatorBaseChar::GetHealth() const
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetHealth();
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

void AGladiatorBaseChar::SetTestAbilities()
{
	if (!AbilitySystemComponent) return;

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, 1,INDEX_NONE, this));
		}
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

void AGladiatorBaseChar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); //call to parent implementation

	if (!AbilitySystemComponent) return;

	if (enableTestAbilities) SetTestAbilities(); //sets up for testing grounds

	
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	//applying default attribute effects to the characters ability system component
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext(); 
	EffectContext.AddSourceObject(this);
	
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, 1, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle activeHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}

	
}

