// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GEC_MeleeDamage.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct MeleeDamageStats
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Toughness);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);

	//declaring how we want to capture the Attributes declared 
	MeleeDamageStats()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGladiatorAttributeSet, Toughness, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGladiatorAttributeSet, BaseDamage, Source, false);
	}
};

//defining a constructor for the stats struct created so information can be parsed through correctly
static const MeleeDamageStats& MDS()
{
	static MeleeDamageStats stats;
	return stats;
};

UGEC_MeleeDamage::UGEC_MeleeDamage()
{
	RelevantAttributesToCapture.Add(MeleeDamageStats().ToughnessDef);
	RelevantAttributesToCapture.Add(MeleeDamageStats().BaseDamageDef);
}

void UGEC_MeleeDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	//ggetting each Ability system components from either end
	UAbilitySystemComponent* SourceAbilitySystemComp = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComp = ExecutionParams.GetTargetAbilitySystemComponent();

	//setting up the actor for either source and target using AvatarActor as when setting up the ASC it is a gaurentee that the avatar actor will be the actor itself
	AActor* sourceActor = SourceAbilitySystemComp ? SourceAbilitySystemComp->GetAvatarActor() : nullptr;
	AActor* targetActor = TargetAbilitySystemComp ? TargetAbilitySystemComp->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//getting the owned tags for target and source as it can affect which buffs to use
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParametres;
	EvaluationParametres.SourceTags = SourceTags;
	EvaluationParametres.TargetTags = TargetTags;

	//defining the toughness attribute when it gets captured and rounding it to a higher value
	float toughness = 0.f;
	{
		const FGameplayAttribute ToughnessAttribute = UGladiatorAttributeSet::GetToughnessAttribute(); //defining what attribute we want to get
		const FGameplayEffectAttributeCaptureDefinition AttributeCaptureDef(ToughnessAttribute, EGameplayEffectAttributeCaptureSource::Target, false); //grabbing the toughness attribute from target
		
		const bool bEvaluated = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeCaptureDef, EvaluationParametres, toughness);

		toughness = FMath::Max<float>(toughness, 0.f); //ensure it doesnt hit a negative number

		UE_LOG(LogTemp, Warning, TEXT("Damage resistance: %f"), toughness);
	}
	

	//defining the BaseDamage attribute when it gets captured and rounding it to a higher value
	float baseDamage = 0.f;
	{
		const FGameplayAttribute BaseDamageAttribute = UGladiatorAttributeSet::GetBaseDamageAttribute();
		const FGameplayEffectAttributeCaptureDefinition AttributeCaptureDef(BaseDamageAttribute, EGameplayEffectAttributeCaptureSource::Source, false);
		
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeCaptureDef, EvaluationParametres, baseDamage);
		baseDamage = FMath::Max<float>(baseDamage, 0.f);	
		
		UE_LOG(LogTemp, Warning, TEXT("Base Damage Retrieved: %f"), baseDamage);
	}
	

	//Damage calculation
	float UnMitigatedDamage = baseDamage;

	float MitigatedDamage = (UnMitigatedDamage) / (1 + toughness * 0.01);

	UE_LOG(LogTemp, Warning, TEXT("Total Damage Calc: %f"), MitigatedDamage);
	
	if (MitigatedDamage > 0.f)
	{
		//setting the Target's damage meta attribute that will impact its health upon recieving it
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UGladiatorAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, MitigatedDamage));
	}
	
}
