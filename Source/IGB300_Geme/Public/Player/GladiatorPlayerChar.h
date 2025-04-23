// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorBaseChar.h"
#include "GameFramework/Character.h"
#include "GladiatorPlayerChar.generated.h"

UCLASS()
class IGB300_GEME_API AGladiatorPlayerChar : public AGladiatorBaseChar
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGladiatorPlayerChar();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilitySystemComp();
	void InitHUD() const;
};
