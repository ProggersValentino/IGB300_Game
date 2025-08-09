// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayStats.h"

int UGameplayStats::GetTotalKills()
{
	return TotalKills;
}

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

