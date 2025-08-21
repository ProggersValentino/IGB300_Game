// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ComboContainer.h"

#include "GladiatorBaseChar.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GladiatorPlayerChar.h"
#include "Player/GladiatorPlayerState.h"
#include "Stats/StatsData.h"

void UComboContainer::Init(UGladiatorAbilitySystemComponent* abilityComp)
{
	/*WorldRef = world;*/
	selectedCharacter = abilityComp;

	CrowdSubsystem = GetWorld()->GetSubsystem<UCrowdWorldSubsystem>(); //get access to crowd subsystem
	
	RefreshActivationGoal();
	
	
	/*if we have a subchain abiltiy then we init that*/
	if (!IsValid(SubChainAbilityClass)) return;
	subChainAbility = NewObject<UComboContainer>(this, SubChainAbilityClass);
	subChainAbility->Init(abilityComp);

	
	
}

void UComboContainer::ExecuteCombo()
{
	/*abilitySpec = MakeAbilitySpec(ChosenAbility, MakeEffectContext());*/
	
	//activate 
	if (IsValid(subChainAbility) && currentHitStreak == SubchainActivationGoal)
	{
		/*abilitySpec.Ability = subChainAbility->ChosenAbility;*/
		selectedCharacter->TryActivateAbilityByClass(subChainAbility->ChosenAbility);
		RefreshActivationGoal();
		return;
	} 
	
	selectedCharacter->TryActivateAbilityByClass(ChosenAbility);
	
}

void UComboContainer::InjectExecuteGameplayEvents(TArray<FHitResult> result)
{
	FGameplayEventData eventData;
	eventData.Instigator = selectedCharacter->GetOwnerActor();
	
	
	/*apply effects to all results*/
	for (FHitResult hitActor : result)
	{
		FGladiatorGameplayEffectContext* newContext;
		MakeEffectContext(newContext);
		
		FGameplayEffectContextHandle Handle(newContext); //make the handle responsible for the lifetime of the newContext
		eventData.ContextHandle = Handle;
		Handle.Get()->AddHitResult(hitActor);
		eventData.ContextHandle = Handle;

		eventData.Target = hitActor.GetActor();
		eventData.TargetData = CreateTargetDataFromHit(hitActor);
		selectedCharacter->HandleGameplayEvent(gameplayEventTag, &eventData);

		//process hit to the crowd subsystem
		CrowdSubsystem->UpdateExcitement(CrowdInfluencePerHit);
		
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

FGameplayEffectSpec UComboContainer::MakeEffectSpec(FGladiatorGameplayEffectContext contextHandle)
{
	AGladiatorPlayerChar* Player = Cast<AGladiatorPlayerChar, AActor>( selectedCharacter->GetAvatarActor());

	
	

	
	FGameplayEffectSpec effectSpc = FGameplayEffectSpec();
	
	return effectSpc;
}

void UComboContainer::MakeEffectContext(FGladiatorGameplayEffectContext*& contextRef)
{
	/*allocate new context on the heap*/
	FGladiatorGameplayEffectContext* context_allo = new FGladiatorGameplayEffectContext(selectedCharacter->GetAvatarActor(), selectedCharacter->GetAvatarActor());

	context_allo->Data_Knockack_verticalForce = VerticalForce;
	context_allo->Data_Knockack_HorrizontalForce = HorizontalForce;
	
	contextRef = context_allo;
	
}


