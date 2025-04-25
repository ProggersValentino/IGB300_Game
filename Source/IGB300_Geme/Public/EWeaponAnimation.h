// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponAnimation.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponAnimation: uint8
{
	AttackDefault = 0 UMETA(DisplayName = "Attack_Default"),
	AttackCombo_1 = 1 UMETA(DisplayName = "Attack_Combo_1"),
	AttackCombo_2 = 2 UMETA(DisplayName = "Attack_Combo_2"),
	AttackCombo_3 = 3 UMETA(DisplayName = "Attack_Combo_3"),
	BlockDefault = 4 UMETA(DisplayName = "Block_Default")
};
