// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrowdWorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "StatAdjustment.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "ComboContainer.generated.h"

UENUM(BlueprintType)
enum class EAbilityComboType: uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Timed = 1 UMETA(DisplayName = "Timed"),
};



/**
 * 
 */
UCLASS(Blueprintable)
class IGB300_GEME_API UComboContainer : public UObject
{
	GENERATED_BODY()
public:
	void Init(UGladiatorAbilitySystemComponent* abilityComp);

	/*executes the ability selected in ChosenAbility*/
	UFUNCTION(BlueprintCallable)
	void ExecuteCombo();

	/// Inject data and execute a gameplay event from the selected gameplay tag ->mostly used on the anim notfiy
	/// @param result the array of results retrieved from a collision trace
	UFUNCTION(BlueprintCallable)
	void InjectExecuteGameplayEvents(TArray<FHitResult> result);
	
	
	UFUNCTION(BlueprintCallable)
	void AddHitStreak(int amount);

	UFUNCTION(BlueprintCallable)
	void ClearStreak();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> ChosenAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability" , meta = (
		ToolTip="if you want to have an ability that plays after the chosenAbility has had a successful hit e.g. chosenAbility is left strike and subChainAbility is right strike"))
	TSubclassOf<UComboContainer> SubChainAbilityClass;

	UPROPERTY()
	TObjectPtr<UComboContainer> subChainAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	int SubChainActivationCriteria;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta = (
		ToolTip="The number of hits required for the player to land in a row before being able to continue to the next combo in the chain"))
	int NOOfHitsToNextComboCriteria;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slowmo")
	bool bSlowGameOnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slowmo", meta = (EditCondition = "bSlowGameOnHit", ClampMin="0.05", ClampMax="1.0"))
	float SlowTimeDilationTo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slowmo", meta = (EditCondition = "bSlowGameOnHit", ClampMin="0.1", ClampMax="1.0"))
	float SlowmoTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta = (ToolTip="The tags to call to fulfill a gameplay event waiting on the abiltiy"))
	FGameplayTag gameplayEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	EAbilityComboType AbilityComboType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability",meta = (EditCondition="bIsTimedCombo", EditConditionHides, ToolTip="the value that determines how much the game slows down"))
	float gameplaySlowdownValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability",meta = (EditCondition="bIsTimedCombo", EditConditionHides,
		ToolTip="the amount of time given to press the button otherwise combo will miss"))
	float timeToPress;

	UPROPERTY(BlueprintType, BlueprintReadOnly)
	int currentHitStreak;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crowd Influence", meta=(ToolTip="When the player hits something how much excitment does the crowd increase by"))
	float CrowdInfluencePerHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	float VerticalForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	float HorizontalForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Impact Freeze")
	bool bImpactFreezeOnHit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Impact Freeze", meta = (EditCondition = "bImpactFreezeOnHit", ClampMin="0.0", ClampMax="1.0"))
	float ImpactFreezeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stat Adjustments", meta=(ToolTip="Added on top of the base damage of the player when attacking an enemy"))
	float AdditionalDamage;
	
private:
	/*allows control of who can access it where its an enemy or player granted its stemmed from the GladiatorBaseCharacter class*/
	UPROPERTY(BlueprintType)
	UGladiatorAbilitySystemComponent* selectedCharacter;
	
	UPROPERTY()
	bool bIsTimedCombo;
	
	UCrowdWorldSubsystem* CrowdSubsystem;
	
	int SubchainActivationGoal;

	/*when a new benchmark needs to be set for the next time the subchain will activate*/
	void RefreshActivationGoal();
	
	FGameplayAbilityTargetDataHandle CreateTargetDataFromHit(const FHitResult& hit);

	UStatAdjustment* statAdjustmentObject;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		bIsTimedCombo = AbilityComboType == EAbilityComboType::Timed;
	}
#endif
	
	FTimerHandle SlowTimeTimerHandle;
	
	void SlowGameOnHit(float slowTimeDilationTo, float slowmoTime);

	void OnTimerEnd();

	FGameplayEffectSpec MakeEffectSpec(FGladiatorGameplayEffectContext contextHandle);

	void MakeEffectContext(FGladiatorGameplayEffectContext*& contextRef);

	//freeze framing
	FTimerHandle FreezeFrameTimerHandle;
	
	void PauseAnimationPlaying(USkeletalMeshComponent* mesh);

	void SetActorTimeDialation(AActor* actor, float timeDialation);
	
	void ResumeAnimationPlaying(USkeletalMeshComponent* mesh);

	void PauseMultipleAnimationsPlaying(USkeletalMeshComponent* owner, USkeletalMeshComponent* target);
	void ResumeMultipleAnimationsPlaying(USkeletalMeshComponent* owner, USkeletalMeshComponent* target);

	void SetMultipleActorTime(AActor* owner, AActor* target, float timeDialation);
	
	//Freezes animations of the owner dishing out the attack and the target its hitting to create a impact freeze effect
	void FreezeFrameAnimationsPlaying(ACharacter* owner, ACharacter* target);
	void FreezeFrameAnimationsPlaying(AActor* owner, AActor* target);
	
};
