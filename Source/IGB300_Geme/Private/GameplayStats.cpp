// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayStats.h"

int UGameplayStats::GetTotalKills()
{
	return TotalKills;
}

/*int UGameplayStats::GetGoldEarned()
{
	return Aureus;
}*/

/*int UGameplayStats::GetCrowdScore()
{
	return CumulativeExcitement;
}*/

/*FString UGameplayStats::GetFansGained()
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
}*/

int UGameplayStats::GetChallengesCompleted()
{
	return ChallengesCompleted;
}

float UGameplayStats::GetBloodSpilled()
{
	return BloodSpilled;
}

void UGameplayStats::SetTotalKills(int value)
{
	TotalKills = value;
	CalculateBloodSpilled();
}

void UGameplayStats::ChangeTotalKills(int value)
{
	TotalKills = TotalKills + value;
	CalculateBloodSpilled();
}

void UGameplayStats::ChangeChallengesCompleted(int value)
{
	ChallengesCompleted = ChallengesCompleted + value;
}

void UGameplayStats::ChangeFansGained(int value)
{
	FansGained = FansGained + value;
}

void UGameplayStats::CalculateBloodSpilled()
{
	BloodSpilled = TotalKills*5.2 + ChallengesCompleted;
}

