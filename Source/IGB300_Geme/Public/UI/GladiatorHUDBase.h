// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockedRageAttributeWidget.h"
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CoinUIClass;
	
	UFUNCTION()
	void Death();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual UGloryAttributeWidget* GetGloryAttributeWidget();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGladiatorAttributeWidget> AttributeWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGloryAttributeWidget> GloryAttributeWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBlockedRageAttributeWidget> BlockedRageAttributeWidgetClass;
	
	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UArenaEventWidget> LoadedArenaEventUIClass;*/
	

	

private:
	UPROPERTY()
	UGladiatorAttributeWidget* AttributeWidget;
	UPROPERTY()
	UGloryAttributeWidget* GloryAttributeWidget;
	UPROPERTY()
	UBlockedRageAttributeWidget* BlockedRageAttributeWidget;

	/*UPROPERTY()
	UArenaEventWidget* ArenaEventUI;*/
	
	UPROPERTY()
	UUserWidget* DeathWidget;
	UPROPERTY()
	UUserWidget* CoinWidget;
};
