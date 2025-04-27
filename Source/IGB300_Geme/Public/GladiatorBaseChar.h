// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/GladiatorAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GladiatorBaseChar.generated.h"

/*
 * The purpose of this class is to be the base class for all future characters from different types player's to enemies
 *
 * It holds the foundational functions, variables and Utility functions that every new character created will need to function properly 
*/

class UGameplayAbility;
class UGladiatorAbilitySystemComponent;
class UGladiatorAttributeSet;
class UGameplayEffect; //UE recommends init attributes through GameplayEffect

UCLASS()
class IGB300_GEME_API AGladiatorBaseChar : public ACharacter, public IAbilitySystemInterface
{
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; //

private:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGladiatorBaseChar();

	virtual UGladiatorAttributeSet* GetAttributeSet() const;
	
	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetMaxSpeed() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetBaseDamage() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetMaxBaseDamage() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetToughness() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetMaxToughness() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetGold() const;

	UFUNCTION(BlueprintCallable, Category="GladiatorStats")
	virtual float GetMaxGold() const;
	
	//activate all gameplay abilities that match a given tag
	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	bool activateAbilitiesWithTag(FGameplayTagContainer abilityTag, bool AllowRemoteActivation = true);
	
	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	bool IsAlive();
	
	UFUNCTION()
	void Die();

	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	void DeathCleanup();

	UPROPERTY()
	FGameplayTag DeathTag;

	UPROPERTY()
	UAnimMontage* DeathMontage;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY()
	UGladiatorAbilitySystemComponent* AbilitySystemComponent; //creating an AS Comp

	UPROPERTY()
	class UGladiatorAttributeSet* AttributeSet;

	
	UPROPERTY(EditDefaultsOnly, Category="Gladiator Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gladiator Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void GiveDefaultAbilities();

	UFUNCTION()
	void RemoveAbilities() const;
	
	UFUNCTION()
	void InitDefaultAttributes() const;
};
