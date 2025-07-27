// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaEventWidget.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UArenaEventWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArenaEventText")
	FString EventTitleTxt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArenaEventText")
	FString EventDescTxt;
};
