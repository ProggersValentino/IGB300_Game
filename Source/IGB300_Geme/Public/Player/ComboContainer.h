// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "ComboContainer.generated.h"

UENUM(BlueprintType)
enum class EAbilityComboType: uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Timed = 1 UMETA(DisplayName = "Timed"),
};



/**
 * 
 */
UCLASS(Blueprintable)
class IGB300_GEME_API UComboContainer : public UObject
{
	GENERATED_BODY()
public:
	void Init(UGladiatorAbilitySystemComponent* abilityComp);
	
	UFUNCTION(BlueprintCallable)
	void ExecuteCombo();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container")
	TSubclassOf<UGameplayAbility> ChosenAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container" , meta = (
		ToolTip="if you want to have an ability that plays after the chosenAbility has had a successful hit e.g. chosenAbility is left strike and subChainAbility is right strike"))
	TSubclassOf<UComboContainer> SubChainAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container", meta = (
		ToolTip="The number of hits required for the player to land in a row before being able to continue to the next combo in the chain"))
	int NOOfHitsToNextComboCriteria;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container", meta = (ToolTip="The tags to call to fulfill a gameplay event waiting on the abiltiy"))
	FGameplayTagContainer gameplayEventTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container")
	EAbilityComboType AbilityComboType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container",meta = (EditCondition="bIsTimedCombo", EditConditionHides, ToolTip="the value that determines how much the game slows down"))
	float gameplaySlowdownValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo Container",meta = (EditCondition="bIsTimedCombo", EditConditionHides,
		ToolTip="the amount of time given to press the button otherwise combo will miss"))
	float timeToPress;

	UPROPERTY(BlueprintType, BlueprintReadOnly)
	int currentNumberOfHits;
	
private:
	/*allows control of who can access it where its an enemy or player granted its stemmed from the GladiatorBaseCharacter class*/
	UPROPERTY(BlueprintType)
	UGladiatorAbilitySystemComponent* selectedCharacter;
	
	UPROPERTY()
	bool bIsTimedCombo;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		bIsTimedCombo = AbilityComboType == EAbilityComboType::Timed;
	}
};
