// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CameraSpeedAdjus.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UAnimNotifyState_CameraSpeedAdjus : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AnimNotifyState")
	void Init(USkeletalMeshComponent* mesh);
	
	UFUNCTION(BlueprintCallable, Category = "AnimNotifyState", meta = (ToolTip="when activated will slow or speed up the game depending on the value set in 'speed'"))
	void AdjustWorldSpeed(float speedValue);

	UFUNCTION(BlueprintCallable, Category = "AnimNotifyState")
	void ReturnToDefault();

protected:
	UPROPERTY()
	float defaultSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Speed")
	float speed = 1.f;

	USkeletalMeshComponent* characterMesh;
	
};
