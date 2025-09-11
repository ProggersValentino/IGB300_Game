// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagement/MaestroBase.h"

// Sets default values
AMaestroBase::AMaestroBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMaestroBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaestroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AMaestroBase::GetCurrentWave()
{
	return CurrentWave;
}

int AMaestroBase::GetNumWaves()
{
	return NumWaves;
}

