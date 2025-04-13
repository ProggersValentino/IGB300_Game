// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBaseChar.h"

#include "GAS/GladiatorAbilitySystemComponent.h"

UAbilitySystemComponent* AGladiatorBaseChar::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Sets default values
AGladiatorBaseChar::AGladiatorBaseChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating ability system comp and setting it to be explicity replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//init attribute set
	AttributeSet = CreateDefaultSubobject<UGladiatorAttributeSet>("AttributeSet");
}

// Called when the game starts or when spawned
void AGladiatorBaseChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGladiatorBaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGladiatorBaseChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

