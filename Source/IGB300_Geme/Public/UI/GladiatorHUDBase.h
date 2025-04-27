// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GloryAttributeWidget.h"
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DeathUIClass;
	
	UFUNCTION()
	void Death();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGladiatorAttributeWidget> AttributeWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGloryAttributeWidget> GloryAttributeWidgetClass;

private:
	UPROPERTY()
	UGladiatorAttributeWidget* AttributeWidget;
	UPROPERTY()
	UGloryAttributeWidget* GloryAttributeWidget;
	
	UPROPERTY()
	UUserWidget* DeathWidget;
};
