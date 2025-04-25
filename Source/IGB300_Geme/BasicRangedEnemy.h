// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyType.h"
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

	EEnemyType IsOfType() override;

protected:
	virtual void Tick(float DeltaTime) override;
	void Move_Implementation() override;
	void Attack_Implementation() override;
	virtual void BeginPlay() override;
};
