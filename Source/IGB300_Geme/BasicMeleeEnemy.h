// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BasicMeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API ABasicMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()

	void Move_Implementation() override;
	void Attack_Implementation() override;
	void Die_Implementation() override;
	void Damage_Implementation(float amount) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
