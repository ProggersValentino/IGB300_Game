// Fill out your copyright notice in the Description page of Project Settings.


#include "Crowd.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACrowd::ACrowd()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACrowd::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrowd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACrowd::GetExcitement()
{
	return Excitement;
}

float ACrowd::GetMaxExcitement()
{
	return MaxExcitement;
}

// reset local excitement to zero
void ACrowd::ResetExcitement()
{
	Excitement = 0;
}

float ACrowd::UpdateExcitement(float XP)
{
	//Set Local Excitement
	float NewExcitement = Excitement + XP;
	if(NewExcitement < 0)
	{
		Excitement = 0;
	}
	else 
	{
		Excitement = NewExcitement;
	}

	//Set Cumulative Excitement
	float NewCumulativeExcitement = CumulativeExcitement + XP;
	if(NewCumulativeExcitement < 0)
	{
		CumulativeExcitement = 0;
	}
	else 
	{
		CumulativeExcitement = NewCumulativeExcitement;
	}

	return CumulativeExcitement;
}

bool ACrowd::KeepCoinsActive()
{
	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if(CurrentTime > TimeActive)
	{
		TimeActive = CurrentTime + 10;
		return true;
	}
	return false;
}

