// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBaseChar.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameManagement/MaestroBase.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"
#include "Kismet/GameplayStatics.h"
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

	DeathTag = FGameplayTag::RequestGameplayTag("Gameplay.State.Death");
}

AGladiatorBaseChar::AGladiatorBaseChar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AGladiatorBaseChar::Init()
{
	LightComboChain.Reserve(LightComboChainClasses.Num());

	InitSelectedComboChain(LightComboChainClasses, LightComboChain);
	InitSelectedComboChain(HeavyComboChainClasses, HeavyComboChain);
	InitSelectedComboChain(SpecialComboChainClasses, SpecialComboChain);
	InitSelectedComboChain(UtilityComboChainClasses, UtilityComboChain);
	
	
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

void AGladiatorBaseChar::OnSpeedChanged(const FOnAttributeChangeData& Data)
{
	float newSpeed = Data.NewValue;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	}
}

bool AGladiatorBaseChar::activateAbilitiesWithTag(FGameplayTagContainer abilityTag, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent) return false;

	return AbilitySystemComponent->TryActivateAbilitiesByTag(abilityTag, AllowRemoteActivation);
}

#pragma endregion

bool AGladiatorBaseChar::IsAlive()
{
	return GetHealth() > 0.0f;
}

//Kill character, if its the player then it will be called on the player state (server) otherwise will be called directly on the actor
void AGladiatorBaseChar::Die()
{
	//Dying
	RemoveAbilities();

	const FGameplayAbilitySpec AbilitySpec(KnockOutAbility, 1); //data surrounding for the ability class

	FGameplayAbilitySpec KnowckOutAbilitySpec(KnockOutAbility, 1);
	AbilitySystemComponent->GiveAbilityAndActivateOnce(KnowckOutAbilitySpec);

	//MaestroClass call event
	//Maestro->AchieveRageBaiter();
	

	if (AbilitySystemComponent->IsValidLowLevel())
	{
		AbilitySystemComponent->CancelAbilities();
		FGameplayTagContainer EffectTagsToRemove;

		PostDeathAction();
		
		AbilitySystemComponent->AddLooseGameplayTag(DeathTag); //applies to actor while still active and is not permantely registered on the ASC
	}
	
	//if (DeathMontage) PlayAnimMontage(DeathMontage); //plays a montage of death which then that calls DeathCleanup on AnimNotify
	//else DeathCleanup();
}

void AGladiatorBaseChar::PostDeathAction()
{
	
}


void AGladiatorBaseChar::DeathCleanup()
{
	Destroy();
}

void AGladiatorBaseChar::ResetCombo()
{
	CurrentComboChainIndex = 0;
}

void AGladiatorBaseChar::ActivateCombo(EAttackType attackTypeToRequest, int attackStage)
{

	
	switch (attackTypeToRequest)
	{
	case EAttackType::Light:
		CurrentCombo = LightComboChain[attackStage];
		break;

	case EAttackType::Medium:
		CurrentCombo = HeavyComboChain[attackStage];
		break;

	case EAttackType::Heavy:
		CurrentCombo = SpecialComboChain[attackStage];
		break;
	case EAttackType::Utility:
		CurrentCombo = UtilityComboChain[attackStage];
		break;

	case EAttackType::FollowUp:
		CurrentCombo = LightComboChain[4];
		break;
		
		default:
			CurrentCombo = LightComboChain[0];
			break;
	}
	/*//determine combo
	CurrentCombo = DetermineCombo();*/

	UE_LOG(LogTemp, Warning, TEXT("Combo is: %s"), *CurrentCombo->GetName());
	
	//activate combo
	CurrentCombo->ExecuteCombo();
}

FGameplayTag AGladiatorBaseChar::GetHealthStateTag(float currentHealth, float maxHealth) const
{
	float normalizedHealth = currentHealth / maxHealth;
	
	if (normalizedHealth > 0.5f)
	{
		return FGameplayTag::RequestGameplayTag("Gameplay.State.Healthy");
	}
	else if (normalizedHealth < 0.5f && normalizedHealth > 0.3f)
	{
		return FGameplayTag::RequestGameplayTag("Gameplay.State.SemiHealthy");
	}
	else
	{
		return FGameplayTag::RequestGameplayTag("Gameplay.State.Critical");
	}
}

void AGladiatorBaseChar::OnChageHealthState(const FOnAttributeChangeData& Data)
{
	if (HealthStateTag.IsValid())
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(HealthStateTag); //remove the previous current tag
	}
	
	HealthStateTag = GetHealthStateTag(Data.NewValue, GetAttributeSet()->GetMaxHealth());
	AbilitySystemComponent->AddLooseGameplayTag(HealthStateTag); //apply new one
}

void AGladiatorBaseChar::InitSelectedComboChain(TArray<TSubclassOf<UComboContainer>> comboToInit, TArray<TObjectPtr<class UComboContainer>>& initComboOUT)
{
	for (int i = 0; i < comboToInit.Num(); ++i)
	{
		if (!IsValid(comboToInit[i])) continue;
	
		//initialize combo with refs
		TObjectPtr<UComboContainer> combo = NewObject<UComboContainer>(this, comboToInit[i]);
		
		initComboOUT.AddUnique(combo); 
		combo->Init(AbilitySystemComponent);
	}
}


TObjectPtr<UComboContainer> AGladiatorBaseChar::DetermineCombo()
{
	if (!IsValid(CurrentCombo))
	{
		return LightComboChain[CurrentComboChainIndex];	
	}
	
	if (CurrentCombo->currentHitStreak >= CurrentCombo->NOOfHitsToNextComboCriteria
		&& CurrentComboChainIndex < LightComboChain.Num() - 1) // true 
	{
		CurrentComboChainIndex++;
		CurrentCombo->ClearStreak();
	}
	else if (CurrentComboChainIndex >= LightComboChain.Num() - 1 && !AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Gameplay.State.IsPunching"))) //TODO: establish a dedicated tag for determining when the player's ability is done
	{
		ResetCombo();
		/*UE_LOG(LogTemp, Error, TEXT("current combo index: %d"), CurrentComboChainIndex)*/
	}

	//return selected combo
	return LightComboChain[CurrentComboChainIndex];
}


// Called when the game starts or when spawned
void AGladiatorBaseChar::BeginPlay()
{
	Super::BeginPlay();
	//Maestro = Cast<AMaestroBase>(UGameplayStatics::GetActorOfClass(GetWorld(), maestroClass));
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

void AGladiatorBaseChar::GiveAndActivateIdleAbilities()
{
	check(AbilitySystemComponent);

	if (!HasAuthority()) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : IdleAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1); //data surrounding for the ability class

		FGameplayAbilitySpec IdleAbilitySpec(AbilityClass, 1);
		
		//give idle ability to player and activate it
		AbilitySystemComponent->GiveAbilityAndActivateOnce(IdleAbilitySpec);
	}
}

void AGladiatorBaseChar::RemoveAbilities() const
{
	if (!HasAuthority() || AbilitySystemComponent->IsValidLowLevel()) return;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.SourceObject == this && DefaultAbilities.Contains(Spec.SourceObject->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	//remove abilities from the player's dead body
	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
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




