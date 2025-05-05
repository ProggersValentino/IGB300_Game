// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IEnemy.h"
#include "EnemyType.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoreMinimal.h"
#include "GladiatorBaseChar.h"
#include "GameplayEffectTypes.h"
#include "IFinishable.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

class AEnemyManager;

UCLASS()
class IGB300_GEME_API AEnemyBase : public AGladiatorBaseChar, public IIEnemy, public IIFinishable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, Category = "Members")
	int32 UID;

	UPROPERTY(EditAnywhere, Category = "Members")
	float health;

	UPROPERTY(EditAnywhere, Category = "Members")
	float speed;

	UPROPERTY(EditAnywhere, Category = "Members")
	float crowdXPWhenKilled;

	UPROPERTY(EditAnywhere, Category = "Members")
	float crowdXPWhenFinished;

	float lastTimeHitByplayer;
	float timeAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Members")
	bool canMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Members")
	FVector targetMovePos;

	AEnemyManager* enemyManager;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Attack_Implementation();
	virtual void Move_Implementation();
	virtual void Damage_Implementation(float amount);
	virtual void Block_Implementation() override;
	virtual bool CanDoFinisher();
	virtual bool CanFinish_Implementation();
	virtual void GetExecuted_Implementation(UAnimMontage* animation) override;
	UFUNCTION(BlueprintCallable)
	virtual EEnemyType IsOfType();

	FDelegateHandle HealthChangeDelegate;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
};
