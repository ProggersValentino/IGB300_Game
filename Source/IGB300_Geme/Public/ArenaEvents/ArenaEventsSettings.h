// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventArenaData.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaEventsSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName="Arena Events")
class IGB300_GEME_API UArenaEventsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	
	public:
	UPROPERTY(config, EditDefaultsOnly, Category = "ArenaEvents")
	TArray<TSoftObjectPtr<UEventArenaData>> ArenaEventsSoftObjectPath;
};
