// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "TankEnemy.generated.h"

/**
 * 
 */
UCLASS()
class IGB300_GEME_API ATankEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
	void Move() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
