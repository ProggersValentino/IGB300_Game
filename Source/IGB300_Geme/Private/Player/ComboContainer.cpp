// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ComboContainer.h"

#include "GladiatorBaseChar.h"
#include "Kismet/GameplayStatics.h"
#include "Stats/StatsData.h"

void UComboContainer::Init(UGladiatorAbilitySystemComponent* abilityComp)
{
	/*WorldRef = world;*/
	selectedCharacter = abilityComp;

	RefreshActivationGoal();
	
	/*if we have a subchain abiltiy then we init that*/
	if (!IsValid(SubChainAbilityClass)) return;
	subChainAbility = NewObject<UComboContainer>(this, SubChainAbilityClass);
	subChainAbility->Init(abilityComp);

	
	
}

void UComboContainer::ExecuteCombo()
{
	//activate 
	if (IsValid(subChainAbility) && currentHitStreak == SubchainActivationGoal)
	{
		selectedCharacter->TryActivateAbilityByClass(subChainAbility->ChosenAbility);
		RefreshActivationGoal();
		return;
	} 
	
	selectedCharacter->TryActivateAbilityByClass(ChosenAbility);
}

void UComboContainer::InjectExecuteGameplayEvents(TArray<FHitResult> result)
{
	FGameplayEventData eventData;
	eventData.Instigator = selectedCharacter->GetOwner();

	/*apply effects to all results*/
	for (FHitResult hitActor : result)
	{
		eventData.Target = hitActor.GetActor();
		eventData.TargetData = CreateTargetDataFromHit(hitActor);
		selectedCharacter->HandleGameplayEvent(gameplayEventTag, &eventData);

		if (bImpactFreezeOnHit)
		{
			SlowGameOnHit(SlowTimeDilationTo, SlowmoTime);
		}
	}
	
}

void UComboContainer::AddHitStreak(int amount)
{
	currentHitStreak += amount;
	UE_LOG(LogTemp, Warning, TEXT("current streaks: %d"), currentHitStreak)
}

void UComboContainer::ClearStreak()
{
	currentHitStreak = 0;
}

void UComboContainer::RefreshActivationGoal()
{
	SubchainActivationGoal = currentHitStreak + SubChainActivationCriteria;
}

FGameplayAbilityTargetDataHandle UComboContainer::CreateTargetDataFromHit(const FHitResult& hit)
{
	FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(hit);
	
	return FGameplayAbilityTargetDataHandle(targetData); /*the targetdatahandle assumes ownership over the targetData created from the 'new' instance thus handles the cleanup*/
}

void UComboContainer::SlowGameOnHit(float slowTimeDilationTo, float slowmoTime)
{
	//need to readjust how long the slowmo lasts based on what dilation gets set for the world 
	float slowmoTimeReadjusted = slowmoTime * slowTimeDilationTo;
	
	UGameplayStatics::SetGlobalTimeDilation(GetOuter()->GetWorld(), slowTimeDilationTo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UComboContainer::OnTimerEnd, slowmoTimeReadjusted, false);
}

void UComboContainer::OnTimerEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


