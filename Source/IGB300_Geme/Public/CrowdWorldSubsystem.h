// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CrowdWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UCrowdWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
    
    public:	
		// Functions for Excitement
		UFUNCTION(BlueprintCallable)
		virtual void ResetExcitement();
    
		UFUNCTION(BlueprintCallable)
		virtual float UpdateExcitement(float XP);
    
		UFUNCTION(BlueprintCallable)
		virtual bool KeepCoinsActive();

		UFUNCTION(BlueprintCallable)
		virtual void SetCrowdSize(int size);

		UFUNCTION(BlueprintCallable)
		virtual void UpdateAureus(int amount);
    
    	UFUNCTION(BlueprintCallable, BlueprintPure)
    	virtual float GetExcitement();
    
    	UFUNCTION(BlueprintCallable, BlueprintPure)
    	virtual float GetMaxExcitement();

		UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual int GetCrowdSize();

		UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual float GetCumulativeExcitement();
    
    	//Excitement variables
    	UPROPERTY(VisibleAnywhere, Category="Crowd")
    	float Excitement;
    
    	UPROPERTY(VisibleAnywhere, Category="Crowd")
    	float CumulativeExcitement;
    
    	UPROPERTY(EditAnywhere, Category="Crowd")
    	float MaxExcitement = 50;
    
    	// Coin variables
    	UPROPERTY(VisibleAnywhere, Category="Coin")
    	float Aureus;
    
    	UPROPERTY(EditAnywhere, Category="Coin")
    	float TimeActive;
    	
    	UPROPERTY(EditAnywhere, Category="Crowd")
        int CrowdSize = 50;

		UPROPERTY(VisibleAnywhere, Category="Crowd")
		int InitCrowdSize = 50;	
};
