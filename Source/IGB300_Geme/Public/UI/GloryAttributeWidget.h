// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GloryAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGloryAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindAttributes();

protected:
	UPROPERTY(BlueprintReadOnly)
	float GloryPercent;
	
};
