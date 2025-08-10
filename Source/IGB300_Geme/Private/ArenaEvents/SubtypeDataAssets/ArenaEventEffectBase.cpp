// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaEvents/SubtypeDataAssets/ArenaEventEffectBase.h"

void UArenaEventEffectBase::Init(UWorld* currentWorld)
{
	WorldRef = currentWorld;
}

AActor* UArenaEventEffectBase::SpawnActorOfClass(UClass* actorToSpawn, FTransform const& SpawnTransform)
{
	return WorldRef->SpawnActor<AActor>(actorToSpawn, SpawnTransform);
}


