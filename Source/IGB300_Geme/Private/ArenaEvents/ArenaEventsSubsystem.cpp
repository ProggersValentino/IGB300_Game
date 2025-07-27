// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaEvents/ArenaEventsSubsystem.h"

#include "ArenaEvents/ArenaEventsSettings.h"
#include "Engine/AssetManager.h"

void UArenaEventsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	/*if we are in game editor dont execute -> this subsystem initialises at any time a world is loaded (including when the editor loads)*/
	if (!GetWorld()->IsGameWorld())
	{
		return;
	}

	Init();	
	
	//are we testing or not? 
	if (GetDefault<UArenaEventsSettings>()->bIsTesting)
	{
		TestSingleEvent(GetDefault<UArenaEventsSettings>()->TestArenaEvent);
	}
	
	
}

void UArenaEventsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UArenaEventsSubsystem::GenerateActivateArenaEvent()
{
	if (!CurrentActiveEvent)
	{
		CurrentActiveEvent = GenerateArenaEvent();
	}

	ActivateArenaEvent(CurrentActiveEvent);
}

void UArenaEventsSubsystem::Init()
{
	UnloadedEvents = GetDefault<UArenaEventsSettings>()->ArenaEventsSoftObjectPath;
	UnloadedArenaEventUI = GetDefault<UArenaEventsSettings>()->ArenaEventUI;

	/*loading the event UI class */
	LoadSingleItemAsync<void>(UnloadedArenaEventUI.ToSoftObjectPath(), [&]()
	{
		TSubclassOf<UArenaEventWidget> loadedWidgetClass = UnloadedArenaEventUI.Get();

		if (loadedWidgetClass)
		{
			LoadedArenaEventUIClass = loadedWidgetClass;
		}
	});
	
	LoadGroupItemsAsync();
}

void UArenaEventsSubsystem::TestSingleEvent(TSoftObjectPtr<UEventArenaData> singleEvent)
{
	UEventArenaData* loadedEvent = LoadItemSynchronous<UEventArenaData>(singleEvent.ToSoftObjectPath());

	if(!singleEvent)
	{
		return;
	}
	
	ActivateArenaEvent(loadedEvent);
}



void UArenaEventsSubsystem::ActivateArenaEvent(UEventArenaData* arenaEventData)
{

	if (!arenaEventData)
	{
		return;
	}
	
	//spawn event effects
	for (TSubclassOf<UArenaEventEffectBase> effect : arenaEventData->Effects)
	{
		UArenaEventEffectBase* bpObject = NewObject<UArenaEventEffectBase>(this, effect);
		bpObject->Init(GetWorld()); //initialize object with world so if can

		//activate effect
		bpObject->ExecuteActivity();

		//cache active event
		activeEffectsCache.AddUnique(bpObject);
	}
	
	//TODO: fix the race condition with the UI loading 
	//activate UI call
	/*ArenaEventUI = CreateWidget<UArenaEventWidget>(GetWorld(), LoadedArenaEventUIClass);
	ArenaEventUI->EventTitleTxt = arenaEventData->EventName;
	ArenaEventUI->EventDescTxt = arenaEventData->EventDesc;
	ArenaEventUI->AddToViewport();*/
	
}

UEventArenaData* UArenaEventsSubsystem::GenerateArenaEvent()
{
	if (ArenaEvents.Num() == 0) return nullptr;
	int areneEventIdx = FMath::RandRange(0, ArenaEvents.Num() - 1);

	UEventArenaData* selectedEvent = ArenaEvents[areneEventIdx];

	return selectedEvent;
}

void UArenaEventsSubsystem::LoadGroupItemsAsync()
{
	TArray<FSoftObjectPath> itemsToStream;
	FStreamableManager& streamer = UAssetManager::GetStreamableManager();

	//convert the ptrs to softpaths for the stream
	for (int i = 0; i < UnloadedEvents.Num(); i++)
	{
		itemsToStream.AddUnique(UnloadedEvents[i].ToSoftObjectPath());
	}
	streamer.RequestAsyncLoad(itemsToStream, FStreamableDelegate::CreateUObject(this, &UArenaEventsSubsystem::DeferItems)); //load in the assets ptrs
	
}



void UArenaEventsSubsystem::DeferItems()
{
	for (int i = 0; i < UnloadedEvents.Num(); i++)
	{
		UEventArenaData* deferredLoadedEvent = UnloadedEvents[i].Get();
		
		if (deferredLoadedEvent) //does derefed ptr exist?
		{
			UE_LOG(LogTemp, Display, TEXT("Loaded Event: %s"), *deferredLoadedEvent->EventName)
			ArenaEvents.AddUnique(deferredLoadedEvent); //now that the asset path is loaded we can deref the ptr item and get a hard ref of the asset
		} 
	}

	//To cover the edge case of needing to activate an event as soon as the game starts up which ensures no race condition error can occur 
	if (GetDefault<UArenaEventsSettings>()->bActivateOnStartup)
	{
		GenerateActivateArenaEvent();
	}
	
	
}

