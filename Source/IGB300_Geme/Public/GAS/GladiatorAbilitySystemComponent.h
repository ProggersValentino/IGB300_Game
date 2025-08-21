// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FGladiatorGameplayEffectContext.h"
#include "GladiatorAbilitySystemComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecieveDamageDelegate, UGladiatorAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

UCLASS()
class IGB300_GEME_API UGladiatorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	UGladiatorAbilitySystemComponent();

public:

	FRecieveDamageDelegate RecieveDamage;
	
	virtual void ReiceveDamage(UGladiatorAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	const FGladiatorGameplayEffectContext* GetLastHitData() const;

	void SetLastHitData(const FGladiatorGameplayEffectContext* Context);

	
protected:
	/*utility variable for applications that needs to call upon the last thing that hit the object as well as relative data like hit location*/
	const FGladiatorGameplayEffectContext* LastObjectThatHitData;

	
};
