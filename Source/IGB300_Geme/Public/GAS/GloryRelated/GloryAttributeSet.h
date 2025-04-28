// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GloryAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGloryAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGloryAttributeSet();

	//Utility Functions -------
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //get the lifetime of each replicated props

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override; //after the effect has been executed, is there anything we want to happen after

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override; //calculate

	UPROPERTY(BlueprintReadOnly, Category="Glory Attributes", ReplicatedUsing = OnRep_Glory)
	FGameplayAttributeData Glory;
	ATTRIBUTE_ACCESSORS(UGloryAttributeSet, Glory);

	UPROPERTY(BlueprintReadOnly, Category="Glory Attributes", ReplicatedUsing = OnRep_MaxGlory)
	FGameplayAttributeData MaxGlory;
	ATTRIBUTE_ACCESSORS(UGloryAttributeSet, MaxGlory);

	//meta attribute
	UPROPERTY(BlueprintReadOnly, Category="Glory Meta Attributes", ReplicatedUsing = OnRep_mGlory)
	FGameplayAttributeData m_Glory;
	ATTRIBUTE_ACCESSORS(UGloryAttributeSet, m_Glory);

	UFUNCTION()
	void OnRep_Glory(const FGameplayAttributeData& OldGlory) const;

	UFUNCTION()
	void OnRep_MaxGlory(const FGameplayAttributeData& OldMaxGlory) const;

	UFUNCTION()
	void OnRep_mGlory(const FGameplayAttributeData& OldMGlory) const;
};
