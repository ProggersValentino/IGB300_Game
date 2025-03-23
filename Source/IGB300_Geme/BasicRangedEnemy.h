// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BasicRangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API ABasicRangedEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void Tick(float DeltaTime) override;
	void Move_Implementation() override;
	void Attack_Implementation() override;
	void Die_Implementation() override;
	void Damage_Implementation(float amount) override;
	virtual void BeginPlay() override;
	
	
};
