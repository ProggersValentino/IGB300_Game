// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/GladiatorAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "Player/ComboContainer.h"
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
	AGladiatorBaseChar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void Init();
	
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

	void OnSpeedChanged(const FOnAttributeChangeData& Data);
	
	//activate all gameplay abilities that match a given tag
	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	bool activateAbilitiesWithTag(FGameplayTagContainer abilityTag, bool AllowRemoteActivation = true);
	
	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	bool IsAlive();
	
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	UFUNCTION(BlueprintNativeEvent, Category="Gladiator Abilities", meta=(ToolTip="When the character it activates its PostDeathAction. this is called from the base character"))
	void PostDeathAction();
	
	UFUNCTION(BlueprintCallable, Category="Gladiator Abilities")
	void DeathCleanup();

	UPROPERTY()
	FGameplayTag HealthStateTag;
	
	UPROPERTY()
	FGameplayTag DeathTag;

	UPROPERTY()
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UComboContainer>> MainComboChainClasses;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UComboContainer> CurrentCombo;

	UFUNCTION(BlueprintCallable, Category="Gladiator Combo")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, Category="Gladiator Combo")
	void ActivateCombo();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY()
	UGladiatorAbilitySystemComponent* AbilitySystemComponent; //creating an AS Comp

	UPROPERTY()
	class UGladiatorAttributeSet* AttributeSet;

	
	UPROPERTY(EditDefaultsOnly, Category="Gladiator Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Gladiator Abilities", meta =(ToolTip="Abilities that are activated upon given to the player and remain idle behind the scenes waiting gameplay event calls"))
	TArray<TSubclassOf<UGameplayAbility>> IdleAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gladiator Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Reactions")
	UAnimMontage* BlockRecoilReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Reactions")
	UAnimMontage* BraceBlockReaction;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void GiveDefaultAbilities();

	UFUNCTION()
	void GiveAndActivateIdleAbilities();

	UFUNCTION()
	void RemoveAbilities() const;
	
	UFUNCTION()
	void InitDefaultAttributes() const;

	void OnChageHealthState(const FOnAttributeChangeData& Data);

protected:
	FGameplayTag GetHealthStateTag(float currentHealth, float maxHealth) const;
	
	
private:
	UPROPERTY()
	TArray<TObjectPtr<class UComboContainer>> MainComboChain;

	int CurrentComboChainIndex;

	
	
	/// 
	/// @return returns the current combo based of the CurrentComboChainIn
	TObjectPtr<UComboContainer> DetermineCombo();
};
