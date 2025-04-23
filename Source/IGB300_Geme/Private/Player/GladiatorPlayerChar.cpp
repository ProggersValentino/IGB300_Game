// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerChar.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "Player/GladiatorPlayerController.h"
#include "Player/GladiatorPlayerState.h"
#include "UI/GladiatorHUDBase.h"

// Sets default values
AGladiatorPlayerChar::AGladiatorPlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGladiatorPlayerChar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); //call to parent implementation

	InitAbilitySystemComp();
	
	
	if (!AbilitySystemComponent) return;

	GiveDefaultAbilities();
	InitDefaultAttributes();
	InitHUD();
	
}

void AGladiatorPlayerChar::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComp();
	InitDefaultAttributes();
	InitHUD();
}



/// Initializing the Ability system comp on the player character through extracting from the player state
void AGladiatorPlayerChar::InitAbilitySystemComp()
{
	AGladiatorPlayerState* playerState = GetPlayerState<AGladiatorPlayerState>(); //we initializing the AbilitySystemComp on the player state therefore we need to get access to it

	check(playerState); //can we safely dereference the pointer? 

	AbilitySystemComponent = CastChecked<UGladiatorAbilitySystemComponent>(
		playerState->GetAbilitySystemComponent());

	AbilitySystemComponent->InitAbilityActorInfo(playerState, this);

	AttributeSet = playerState->GetAttributeSet();

	FString Message = FString::Printf(TEXT("playerstate: %s"), *playerState->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
}

void AGladiatorPlayerChar::InitHUD() const
{
	if (const AGladiatorPlayerController* playerController = Cast<AGladiatorPlayerController>(GetController()))
	{
		if (AGladiatorHUDBase* GladHUD = Cast<AGladiatorHUDBase>(playerController->GetHUD()))
		{
			GladHUD->Init();
		}
	}
}
