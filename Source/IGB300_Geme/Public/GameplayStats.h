// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrowdWorldSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayStats.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGameplayStats : public UCrowdWorldSubsystem
{
	GENERATED_BODY()

	public:
	UPROPERTY()
	int TotalKills;

	UPROPERTY()
	int FansGained;

	UPROPERTY()
	int ChallengesCompleted;

	UPROPERTY()
	float BloodSpilled;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetTotalKills();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetGoldEarned();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetCrowdScore();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetFansGained();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetChallengesCompleted();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetBloodSpilled();

	UFUNCTION(BlueprintCallable)
	virtual void SetTotalKills(int value);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeTotalKills(int value);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeChallengesCompleted(int value);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeFansGained(int value);

	UFUNCTION(BlueprintCallable)
	virtual void CalculateBloodSpilled();
};
