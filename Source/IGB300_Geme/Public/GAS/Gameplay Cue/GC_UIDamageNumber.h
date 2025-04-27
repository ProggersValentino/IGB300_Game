// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "UI/DamageNumberWidget_Base.h"
#include "GC_UIDamageNumber.generated.h"

/**
 * The base class for all types damage number Gameplay cues which when created is subclassed off this class 
 */
UCLASS()
class IGB300_GEME_API AGC_UIDamageNumber : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

public:

	//player controller so we can apply an ownerObject to the widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	APlayerController* PlayerController;

	//widget setup
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDamageNumberWidget_Base> DamageNumberWidgetClass;
	UDamageNumberWidget_Base* DamageNumberWidget;
};
