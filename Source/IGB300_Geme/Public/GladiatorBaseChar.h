// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/GladiatorAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GladiatorBaseChar.generated.h"

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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent; //creating an AS Comp

	UPROPERTY()
	class UGladiatorAttributeSet* AttributeSet;

	//the level of the gladiator -> should not be changed directly during runtime
	UPROPERTY(EditAnywhere, Category = "Gladiator Abilities")
	int32 GladiatorLvl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gladiator Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Abilities|Debug")
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;

	virtual void SetTestAbilities();

	//if you want to test abilities set this to true
	UPROPERTY(EditAnywhere, Category = "Gladiator Abilities|Debug")
	bool enableTestAbilities;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
};
