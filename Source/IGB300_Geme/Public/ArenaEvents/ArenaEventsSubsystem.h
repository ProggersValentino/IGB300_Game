// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventArenaData.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArenaEventsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UArenaEventsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArenaEvents")
	TArray<UEventArenaData*> ArenaEvents;

private:
	
	void Init();

	template<typename TSoftInput, typename TSoftOutput>
	TSoftOutput* LoadFromSettings(TSoftInput objectToLoad);
};

template <typename TSoftInput, typename TSoftOutput>
TSoftOutput* UArenaEventsSubsystem::LoadFromSettings(TSoftInput objectToLoad)
{
	FStreamableManager& streamer = UAssetManager::GetStreamableManager();

	TSoftInput loaded = streamer.LoadSynchronous(objectToLoad, true);

	if (loaded) return loaded;

	return nullptr;
}
