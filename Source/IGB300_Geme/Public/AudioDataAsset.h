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
 				 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetName;
 				 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* AssetSoundWave;
 				 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMetaSoundSource* MetaSoundSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent; 
};

UCLASS(Blueprintable)
class IGB300_GEME_API UAudioDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FAudioInfo> AudioItems;
	
};
