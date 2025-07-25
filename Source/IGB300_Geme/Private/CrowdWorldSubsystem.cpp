// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"

float UCrowdWorldSubsystem::GetExcitement()
{
	return Excitement;
}

float UCrowdWorldSubsystem::GetMaxExcitement()
{
	return MaxExcitement;
}

// reset local excitement to zero
void UCrowdWorldSubsystem::ResetExcitement()
{
	Excitement = 0;
}

float UCrowdWorldSubsystem::UpdateExcitement(float XP)
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

bool UCrowdWorldSubsystem::KeepCoinsActive()
{
	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if(CurrentTime > TimeActive)
	{
		TimeActive = CurrentTime + 10;
		return true;
	}
	return false;
}

int UCrowdWorldSubsystem::GetCrowdSize()
{
	return CrowdSize;
}

void UCrowdWorldSubsystem::SetCrowdSize(int size)
{
	CrowdSize = size;
}