// Fill out your copyright notice in the Description page of Project Settings.


#include "LootDataAsset.h"

UFUNCTION(BlueprintCallable)
static FLootAssetInfo GetRandomLootItem(UDataAsset* LootDataAsset)
{
    ULootDataAsset* LootData = Cast<ULootDataAsset>(LootDataAsset);
    if (!LootData || LootData->AssetItems.Num() == 0) return FLootAssetInfo();

    int32 Index = FMath::RandRange(0, LootData->AssetItems.Num() - 1);
    return LootData->AssetItems[Index];
}