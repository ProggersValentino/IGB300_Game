// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWeapon.h"
#include "SWeaponData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS(Blueprintable)
class IGB300_GEME_API AWeaponBase : public AActor, public IIWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FSWeaponData WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FDataTableRowHandle DataSource;

	//to implement an Interface in UE you need "NameOfMethod_Implementation()"
	virtual FSWeaponData AddWeapon_Implementation() override;

	virtual bool DamageObject_Implementation(const AActor* enemy, float damage);
	
};
