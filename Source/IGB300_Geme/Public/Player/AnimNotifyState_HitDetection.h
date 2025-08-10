// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorBaseChar.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_HitDetection.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UAnimNotifyState_HitDetection : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* MeshComponent);
	
	/// Generates and return a trace collision 
	/// @param MeshComponent the mesh component thats activating the notify state
	/// @return 
	UFUNCTION(BlueprintCallable)
	TArray<FHitResult> GenerateTraceCollision(USkeletalMeshComponent* MeshComponent, float radius, FName socketName);

	/*to give a clear signal whether during the ability hit at least one thing over the course of the attack*/
	bool DidAbilityCollide();

	/*modifies the streak depending on whether the ability made collision or not */
	UFUNCTION(BlueprintCallable)
	void ModifyStreak();
	
	///
	///cleanup process at the end of the notify state 
	UFUNCTION(BlueprintCallable)
	void CleanUp();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsHitToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGladiatorBaseChar* character;

private:
	bool didHit;
};
