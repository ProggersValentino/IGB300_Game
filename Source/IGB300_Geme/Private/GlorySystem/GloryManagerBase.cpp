// Fill out your copyright notice in the Description page of Project Settings.


#include "GlorySystem/GloryManagerBase.h"

#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GloryRelated/GloryAttributeSet.h"

UAbilitySystemComponent* AGloryManagerBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGloryAttributeSet* AGloryManagerBase::GetGloryAttributeSet() const
{
	return GloryAttributeSet;
}

// Sets default values
AGloryManagerBase::AGloryManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	GloryAttributeSet = CreateDefaultSubobject<UGloryAttributeSet>("AttributeSet");

}

// Called when the game starts or when spawned
void AGloryManagerBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveDefaultAbilities();
	InitDefaultAttributes();

	//binding to the Glory attribute so when it changes the GloryChanged() function gets called
	OnGloryChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GloryAttributeSet->GetGloryAttribute()).AddUObject(this, &AGloryManagerBase::GloryChanged);
	
}

// Called every frame
void AGloryManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGloryManagerBase::GiveDefaultAbilities()
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

void AGloryManagerBase::RemoveAbilities() const
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

void AGloryManagerBase::InitDefaultAttributes() const
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

		FString Message = FString::Printf(TEXT("new handle was valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
		/*FString Message = FString::Printf(TEXT("new handle was valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, Message);*/

	}
	else
	{
		FString Message = FString::Printf(TEXT("new handle was not valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}
}

void AGloryManagerBase::GloryChanged(const FOnAttributeChangeData& Data)
{
	//apply checks for if the Glory is about to reach another level
}

