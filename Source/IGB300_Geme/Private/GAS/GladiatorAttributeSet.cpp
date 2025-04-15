// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GladiatorAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UGladiatorAttributeSet::UGladiatorAttributeSet()
{
	InitHealth(80.f);
}

void UGladiatorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, MaxSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, BaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, MaxBaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, MaxToughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGladiatorAttributeSet, MaxGold, COND_None, REPNOTIFY_Always);

}

///Generally before changing a new value this method is used for ensuring the value is clamped in the correct range and not over or in the negatives (unless its within the range)
void UGladiatorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxSpeed());
	}
	else if (Attribute == GetBaseDamageAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxBaseDamage());
	}
	else if (Attribute == GetToughnessAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxToughness());
	}
	else if (Attribute == GetGoldAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxGold());
	}
	
}

///Here is where the actual values will be commited to the                               
void UGladiatorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	
	
	//checks for each attribute -> cannot use switch statement due to the const
	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		/*FString Message = FString::Printf(TEXT("Health: %f"), GetHealth());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);*/
		
	}
	else if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		SetSpeed(FMath::Clamp(GetSpeed(), 0.0f, GetMaxSpeed()));
	}
	else if (Attribute == GetBaseDamageAttribute())
	{
		SetBaseDamage(FMath::Clamp(GetBaseDamage(), 0.0f, GetMaxBaseDamage()));
	}
	else if (Attribute == GetToughnessAttribute())
	{
		SetToughness(FMath::Clamp(GetToughness(), 0.0f, GetMaxToughness()));
	}
	else if (Attribute == GetGoldAttribute())
	{
		SetGold(FMath::Clamp(GetGold(), 0.0f, GetMaxGold()));
	}
	
	
	
}

#pragma region Replication Setters
void UGladiatorAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Health, OldHealth);
}

void UGladiatorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxHealth, OldMaxHealth);
}

void UGladiatorAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Speed, OldSpeed);
}

void UGladiatorAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxSpeed, OldMaxSpeed);
}

void UGladiatorAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, BaseDamage, OldBaseDamage);
}

void UGladiatorAttributeSet::OnRep_MaxBaseDamage(const FGameplayAttributeData& OldMaxBaseDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxBaseDamage, OldMaxBaseDamage);
}

void UGladiatorAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Toughness, OldToughness);
}

void UGladiatorAttributeSet::OnRep_MaxToughness(const FGameplayAttributeData& OldMaxToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxToughness, OldMaxToughness);
}

void UGladiatorAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, Gold, OldHealth);
}

void UGladiatorAttributeSet::OnRep_MaxGold(const FGameplayAttributeData& OldMaxGold) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGladiatorAttributeSet, MaxGold, OldMaxGold);
}
#pragma endregion
