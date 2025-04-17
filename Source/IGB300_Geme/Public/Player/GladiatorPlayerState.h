// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GladiatorPlayerState.generated.h"

/**
 * The purpose of this is to house the ability system component and Attributes that will be passed down to characters that extract its values from here
 */

class UGladiatorAbilitySystemComponent; //forward declaration of the class ability system
class UGladiatorAttributeSet;

UCLASS()
class IGB300_GEME_API AGladiatorPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UGladiatorAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetHealth() const;

	FGameplayTag DeathTag;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UGladiatorAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UGladiatorAttributeSet* AttributeSet;
	
	FDelegateHandle HealthChangedDelegate;

	//attribute change callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	bool IsAlive() const;

	

private:
	AGladiatorPlayerState();
};
