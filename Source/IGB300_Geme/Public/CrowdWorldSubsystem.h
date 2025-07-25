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
    
    	UFUNCTION(BlueprintCallable, BlueprintPure)
    	virtual float GetExcitement();
    
    	UFUNCTION(BlueprintCallable, BlueprintPure)
    	virtual float GetMaxExcitement();
    
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
	
};
