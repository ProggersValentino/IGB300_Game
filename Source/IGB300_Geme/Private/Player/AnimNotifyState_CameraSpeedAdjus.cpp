// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifyState_CameraSpeedAdjus.h"

#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_CameraSpeedAdjus::Init(USkeletalMeshComponent* mesh)
{
	if (!IsValid(mesh))
	{
		return;
	}

	characterMesh = mesh;
}

void UAnimNotifyState_CameraSpeedAdjus::AdjustWorldSpeed(float speedValue)
{
	if (!IsValid(characterMesh))
	{
		return;
	}
	
	UGameplayStatics::SetGlobalTimeDilation(characterMesh->GetWorld(), speedValue);
}

void UAnimNotifyState_CameraSpeedAdjus::ReturnToDefault()
{
	if (!IsValid(characterMesh))
	{
		return;
	}
	
	UGameplayStatics::SetGlobalTimeDilation(characterMesh->GetWorld(), defaultSpeed);
}
