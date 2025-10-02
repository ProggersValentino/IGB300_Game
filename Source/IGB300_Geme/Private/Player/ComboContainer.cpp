// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ComboContainer.h"

#include "GladiatorBaseChar.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	statAdjustmentObject = NewObject<UStatAdjustment>();
	
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
	eventData.Instigator = selectedCharacter->GetAvatarActor();

	if (IsValid(statAdjustmentObject))
	{
		statAdjustmentObject->damageAdjustment = AdditionalDamage;
		eventData.OptionalObject = statAdjustmentObject; //adding the stat adjustments to payload
	}
	
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

		UE_LOG(LogTemp, Warning, TEXT("Gameplay Tag to activate gameplay event is: %s"), *gameplayEventTag.ToString());
		
		selectedCharacter->HandleGameplayEvent(gameplayEventTag, &eventData);

		//process hit to the crowd subsystem
		CrowdSubsystem->UpdateExcitement(CrowdInfluencePerHit);
		
		if (bSlowGameOnHit)
		{
			SlowGameOnHit(SlowTimeDilationTo, SlowmoTime);
		}

		if (bImpactFreezeOnHit)
		{
			FreezeFrameAnimationsPlaying(selectedCharacter->GetAvatarActor(), hitActor.GetActor());
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
	GetWorld()->GetTimerManager().SetTimer(SlowTimeTimerHandle, this, &UComboContainer::OnTimerEnd, slowmoTimeReadjusted, false);
}

void UComboContainer::OnTimerEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorld()->GetTimerManager().ClearTimer(SlowTimeTimerHandle);
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

void UComboContainer::PauseAnimationPlaying(USkeletalMeshComponent* mesh)
{
	UAnimInstance* animInstance = Cast<UAnimInstance>(mesh->GetAnimInstance());

	//animInstance->Montage_Pause(); //the pause all current active montage
	animInstance->Montage_SetPlayRate(NULL, 0.01f);
}

void UComboContainer::SetActorTimeDialation(AActor* actor, float timeDialation)
{
	actor->CustomTimeDilation = timeDialation;
}



void UComboContainer::ResumeAnimationPlaying(USkeletalMeshComponent* mesh)
{
	UAnimInstance* animInstance = Cast<UAnimInstance>(mesh->GetAnimInstance());

	//animInstance->Montage_Resume(NULL); //resume all active montages that were paused
	animInstance->Montage_SetPlayRate(NULL, 1.0f);
}

void UComboContainer::PauseMultipleAnimationsPlaying(USkeletalMeshComponent* owner, USkeletalMeshComponent* target)
{
	PauseAnimationPlaying(owner);
	PauseAnimationPlaying(target);
}

void UComboContainer::ResumeMultipleAnimationsPlaying(USkeletalMeshComponent* owner, USkeletalMeshComponent* target)
{
	ResumeAnimationPlaying(target);
	ResumeAnimationPlaying(owner);
}

void UComboContainer::SetMultipleActorTime(AActor* owner, AActor* target, float timeDialation)
{
	SetActorTimeDialation(owner, timeDialation);
	SetActorTimeDialation(target, timeDialation);
}

void UComboContainer::FreezeFrameAnimationsPlaying(ACharacter* owner, ACharacter* target)
{
	USkeletalMeshComponent* ownerMesh = owner->GetMesh();
	USkeletalMeshComponent* targetMesh = target->GetMesh();

	
	//PauseMultipleAnimationsPlaying(ownerMesh, targetMesh);

	FTimerDelegate FreezeFrameTimerDelegate = FTimerDelegate::CreateUObject(this, &UComboContainer::ResumeMultipleAnimationsPlaying, ownerMesh, targetMesh);
	
	GetWorld()->GetTimerManager().SetTimer(FreezeFrameTimerHandle, FreezeFrameTimerDelegate, ImpactFreezeTime, false);
}

void UComboContainer::FreezeFrameAnimationsPlaying(AActor* owner, AActor* target)
{
	SetMultipleActorTime(owner, target, 0.0f);

	AGladiatorPlayerChar* playerChar = Cast<AGladiatorPlayerChar>(owner);
	playerChar->GetCharacterMovement()->Velocity = FVector(0, 0, 0);

	FTimerDelegate FreezeFrameTimerDelegate = FTimerDelegate::CreateUObject(this, &UComboContainer::SetMultipleActorTime, owner, target, 1.0f);
	
	GetWorld()->GetTimerManager().SetTimer(FreezeFrameTimerHandle, FreezeFrameTimerDelegate, ImpactFreezeTime, false);
}




