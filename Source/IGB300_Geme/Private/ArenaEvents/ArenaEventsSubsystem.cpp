// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaEvents/ArenaEventsSubsystem.h"

#include "ArenaEvents/ArenaEventsSettings.h"

void UArenaEventsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UArenaEventsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UArenaEventsSubsystem::Init()
{
	/*ArenaEvents = Cast<TArray<UEventArenaData*>,TArray<UObject*>
	(LoadFromSettings<TArray<TSoftObjectPtr<UEventArenaData>>, TArray<UObject*>>(GetDefault<UArenaEventsSettings>()->ArenaEventsSoftObjectPath));*/
}
