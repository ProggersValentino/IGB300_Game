// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AudioDataAsset.generated.h"

/**
 * 
 */
class UMetaSoundSource;

USTRUCT(Blueprintable)
struct FAudioInfo {
	GENERATED_BODY()
 				 
	UPROPERTY(EditAnywhere)
    FString AssetName;
 				 
	UPROPERTY(EditAnywhere)
	USoundWave* AssetSoundWave;
 				 
	UPROPERTY(EditAnywhere)
	UMetaSoundSource* MetaSoundSource;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent; 
};

UCLASS(Blueprintable)
class IGB300_GEME_API UAudioDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
    TArray<FAudioInfo> AudioItems;
	
};
