// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GladiatorAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGladiatorAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAttributes();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float HealthPercent; //how much health we have left 
};
