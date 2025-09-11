// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorBaseChar.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_GameplayEffect.generated.h"

/**
 * the purpose of this class it to support gameplay effect activation during animations more speficially gameplay effects with duration policies of infinite & Has Duration
 */
UCLASS()
class IGB300_GEME_API UAnimNotifyState_GameplayEffect : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* MeshComp);

	//activates the gameplay effect
	UFUNCTION(BlueprintCallable)
	void InitGameplayEffect(TSubclassOf<UGameplayEffect> effect);

	//removes the gameplay effect at the end of the notify state
	UFUNCTION(BlueprintCallable)
	void RemoveEffect();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	AGladiatorBaseChar* character;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	UGladiatorAbilitySystemComponent* abilitySystem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player")
	TSubclassOf<UGameplayEffect> EffectToPlay;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player")
	FActiveGameplayEffectHandle ActiveGameplayEffect;
};
