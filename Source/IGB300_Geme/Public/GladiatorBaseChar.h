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
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void PossessedBy(AController* NewController) override;
};
