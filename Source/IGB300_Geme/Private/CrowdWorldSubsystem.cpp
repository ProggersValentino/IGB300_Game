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

FString UCrowdWorldSubsystem::GetFansGained()
{
	int fansGained = CrowdSize - InitCrowdSize;
	if (fansGained >= 0 && fansGained < 5)
	{
		return "Parents";
	}
	else if (fansGained >= 5 && fansGained < 10)
	{
		return "You're No.1 Fan";
	}
	else if (fansGained >= 10 && fansGained < 20)
	{
		return "You're No.2 Fan";
	}
	else if (fansGained >= 20 && fansGained < 30)
	{
		return "A True Fan Base";
	}
	else
	{
		return "Everyone!!!";
	}
}

int UCrowdWorldSubsystem::GetAureus()
{
	return Aureus;
}


int UCrowdWorldSubsystem::GetCrowdSize()
{
	return CrowdSize;
}

void UCrowdWorldSubsystem::SetCrowdSize(int size)
{
	CrowdSize = size;
}

float UCrowdWorldSubsystem::GetCumulativeExcitement()
{
	return CumulativeExcitement;
}

void UCrowdWorldSubsystem::UpdateAureus(int amount)
{
	Aureus = Aureus + amount;
}
