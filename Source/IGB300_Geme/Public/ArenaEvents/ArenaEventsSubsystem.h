// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventArenaData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/ArenaEventWidget.h"
#include "UI/GladiatorHUDBase.h"
#include "ArenaEventsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UArenaEventsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArenaEvents")
	TArray<UEventArenaData*> ArenaEvents;

	UEventArenaData* CurrentActiveEvent;
	
	void GenerateActivateArenaEvent();
	
private:
	
	void Init();

	/*when we want to test a single event then we can*/
	void TestSingleEvent(TSoftObjectPtr<UEventArenaData> singleEvent);

	template<typename TSoftOutput>
	TSoftOutput* LoadItemSynchronous(FSoftObjectPath objectToLoad);

	/// activates an inputed event 
	/// @param arenaEventData the selected event wanted for activation
	void ActivateArenaEvent(UEventArenaData* arenaEventData);
	
	/*select and return a random event*/
	UEventArenaData* GenerateArenaEvent();
	
	/*Asynchronously load a group of items from a list of TSoftObjectPtr*/
	void LoadGroupItemsAsync();

	/*load a single item asynchronously while also being able to create a lamda to process the result*/
	template<typename TResult>
	void LoadSingleItemAsync(FSoftObjectPath objectToLoad, TFunction<TResult()> boundFunc);

	/*Deref the loaded ptrs and insert them into an array*/
	void DeferItems();

	UPROPERTY()
	TArray<TSoftObjectPtr<UEventArenaData>> UnloadedEvents;

	UPROPERTY()
	TSoftClassPtr<UArenaEventWidget> UnloadedArenaEventUI;

	UPROPERTY()
	TSubclassOf<UArenaEventWidget> LoadedArenaEventUIClass;

	UArenaEventWidget* ArenaEventUI;

	AGladiatorHUDBase* GladiatorHUD;
	//caching active effects for when we need to cleanup
	UPROPERTY()
	TArray<UArenaEventEffectBase*> activeEffectsCache;
};

template <typename TSoftOutput>
TSoftOutput* UArenaEventsSubsystem::LoadItemSynchronous(FSoftObjectPath objectToLoad)
{
	FStreamableManager& streamer = UAssetManager::GetStreamableManager();

	UObject* uLoaded = streamer.LoadSynchronous(objectToLoad, true);

	if (uLoaded)
	{
		TSoftOutput* output = Cast<TSoftOutput, UObject>(uLoaded);
		return output;
	}

	return nullptr;
}

template<typename TResult>
void UArenaEventsSubsystem::LoadSingleItemAsync(FSoftObjectPath objectToLoad, TFunction<TResult()> boundFunc)
{
	FStreamableManager& streamer = UAssetManager::GetStreamableManager();

	streamer.RequestAsyncLoad(objectToLoad, FStreamableDelegate::CreateLambda(boundFunc));
}