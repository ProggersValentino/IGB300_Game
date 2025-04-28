// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/GladiatorPlayerController.h"
#include "GladiatorBaseChar.h"

void AGladiatorPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	/*AGladiatorBaseChar* CharacterBase = static_cast<AGladiatorBaseChar*>(P); //retrieve reference of player character

	if (!CharacterBase) return;

	CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);*/ //initializing the ability system on the client side
}
