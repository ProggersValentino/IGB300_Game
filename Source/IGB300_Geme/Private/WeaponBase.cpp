// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponData = *DataSource.GetRow<FSWeaponData>(""); //dereferencing the pointer to get the value from the address to apply to WeaponData
	
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FSWeaponData AWeaponBase::AddWeapon_Implementation()
{
	return WeaponData;
}

bool AWeaponBase::DamageObject_Implementation(const AActor* enemy, float damage)
{
	return false; 
}

UAnimMontage* AWeaponBase::RetrieveAnimation_Implementation(EWeaponAnimation selectedAnim)
{
	if (WeaponAnimations.Contains(selectedAnim)) return WeaponAnimations.FindChecked(selectedAnim);

	if (GEngine) GEngine -> AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Can't Find Animation"));
	return nullptr;
}

bool AWeaponBase::BlockIncomingDamage_Implementation()
{
	return IIWeapon::BlockIncomingDamage_Implementation();
}

