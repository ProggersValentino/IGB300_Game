// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GladiatorAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class IGB300_GEME_API UGladiatorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGladiatorAttributeSet();

	//Utility Functions -------
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //get the lifetime of each replicated props

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override; //after the effect has been executed, is there anything we want to happen after

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override; //calculate 


	/// Individual Attributes -------
	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, Speed)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_MaxSpeed)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, MaxSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_BaseDamage)
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, BaseDamage)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_MaxBaseDamage)
	FGameplayAttributeData MaxBaseDamage;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, MaxBaseDamage)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_Toughness)
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, Toughness)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_MaxToughness)
	FGameplayAttributeData MaxToughness;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, MaxToughness)
	
	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, Gold)

	UPROPERTY(BlueprintReadOnly, Category = "GladiatorStats", ReplicatedUsing = OnRep_MaxGold)
	FGameplayAttributeData MaxGold;
	ATTRIBUTE_ACCESSORS(UGladiatorAttributeSet, MaxGold)

	//Replication Functions ------
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldSpeed);

	UFUNCTION()
	virtual void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed);

	UFUNCTION()
	virtual void OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage);

	UFUNCTION()
	virtual void OnRep_MaxBaseDamage(const FGameplayAttributeData& OldMaxBaseDamage);

	UFUNCTION()
	virtual void OnRep_Toughness(const FGameplayAttributeData& OldToughness);

	UFUNCTION()
	virtual void OnRep_MaxToughness(const FGameplayAttributeData& OldMaxToughness);

	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxGold(const FGameplayAttributeData& OldMaxGold);

	
};
