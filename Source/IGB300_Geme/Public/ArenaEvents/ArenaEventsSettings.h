// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventArenaData.h"
#include "Engine/DeveloperSettings.h"
#include "UI/ArenaEventWidget.h"
#include "ArenaEventsSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName="Arena Events")
class IGB300_GEME_API UArenaEventsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	
	public:

	UPROPERTY(config, EditDefaultsOnly, Category = "ArenaEvents", meta = (ToolTip="When true will generate and activate a random event in the given array below as soon as the game starts"))
	bool bActivateOnStartup;
	
	UPROPERTY(config, EditDefaultsOnly, Category = "ArenaEvents")
	TArray<TSoftObjectPtr<UEventArenaData>> ArenaEventsSoftObjectPath;

	UPROPERTY(config, EditDefaultsOnly, Category = "ArenaEvents", meta = (ToolTip="The UI that will display when an arena event is activated"))
	TSoftClassPtr<UArenaEventWidget> ArenaEventUI;
	
	UPROPERTY(config, EditDefaultsOnly, Category = "TestingArenaEvents", meta = (ToolTip="Determines if testing or not, if testing then it will activate the inserted Data Asset"))
	bool bIsTesting;

	UPROPERTY(config, EditDefaultsOnly, Category = "TestingArenaEvents")
	TSoftObjectPtr<UEventArenaData> TestArenaEvent;
};
