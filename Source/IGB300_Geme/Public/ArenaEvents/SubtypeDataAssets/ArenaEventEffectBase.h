// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArenaEventEffectBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IGB300_GEME_API UArenaEventEffectBase : public UObject
{
	GENERATED_BODY()
public:

	void Init(UWorld* currentWorld);
	
	UFUNCTION(BlueprintImplementableEvent, Category="ArenaEvents", meta=(ToolTip="Called when this is spawned into the world"))
	void ExecuteActivity();

	UFUNCTION(BlueprintImplementableEvent, Category="ArenaEvents", meta=(ToolTip="Called when the event is over and needs to cleanup. This might include deletion of actors that are spawned in or "))
	void CleanupActivity();

protected:
	UWorld* WorldRef;
};
