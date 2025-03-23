// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" //this is needed if you want the scripts to compile with data tables
#include "SWeaponData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct IGB300_GEME_API FSWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fCrowdXp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBlock;
};
