// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget_Base.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UDamageNumberWidget_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	//who do we want the widget to apply to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;

	//the damage value to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};
