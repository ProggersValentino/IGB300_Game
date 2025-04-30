// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GAS/GloryRelated/GloryAttributeSet.h"
#include "GloryManagerBase.generated.h"

//forward declare classes for the compiler
class UGameplayAbility;
class UGladiatorAbilitySystemComponent;
class UGloryAttributeSet;
class UGameplayEffect; //UE recommends init attributes through GameplayEffect

UCLASS()
class IGB300_GEME_API AGloryManagerBase : public AActor, public IAbilitySystemInterface
{
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UGloryAttributeSet* GetGloryAttributeSet() const;

private:
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGloryManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UGloryAttributeSet* GloryAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category="Gladiator Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gladiator Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GiveDefaultAbilities();

	UFUNCTION()
	void RemoveAbilities() const;
	
	UFUNCTION()
	void InitDefaultAttributes() const;

	//Increase the overall level of the Glory 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Level And XP")
	bool IncreaseLevel(int AddLevel) __override;

	FDelegateHandle OnGloryChangedDelegate;

	virtual void GloryChanged(const FOnAttributeChangeData& Data);
};
