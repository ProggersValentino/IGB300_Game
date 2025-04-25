// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GladiatorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API AGladiatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	 public:
		virtual void AcknowledgePossession(class APawn* P) override;
};
