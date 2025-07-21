// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubtypeDataAssets/ArenaEventEffectBase.h"
#include "EventArenaData.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UEventArenaData : public UDataAsset
{

	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arena")
	FString EventName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arena")
	FString EventDesc;

	/*for the event if you want to have 1 or multiple effects play out at the activation of the event*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arena")
	TArray<UArenaEventEffectBase*> Effects;
};
