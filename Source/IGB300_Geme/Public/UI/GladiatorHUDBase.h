// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GladiatorHUDBase.generated.h"

/**
 * 
 */

class UGladiatorAttributeWidget;
UCLASS()
class IGB300_GEME_API AGladiatorHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGladiatorAttributeWidget> AttributeWidgetClass;

private:
	UPROPERTY()
	UGladiatorAttributeWidget* AttributeWidget;
};
