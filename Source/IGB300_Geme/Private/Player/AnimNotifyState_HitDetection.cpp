// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifyState_HitDetection.h"

#include "Chaos/PBDSuspensionConstraintData.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_HitDetection::Init(USkeletalMeshComponent* MeshComponent)
{
	if (!IsValid(MeshComponent)) return;
	
	character = Cast<AGladiatorBaseChar>(MeshComponent->GetOwner()); //set the global character var here

	ActorsHitToIgnore.AddUnique(character);
}

TArray<FHitResult> UAnimNotifyState_HitDetection::GenerateTraceCollision(USkeletalMeshComponent* MeshComponent, float radius, FName socketName)
{
	if (!IsValid(MeshComponent) && !IsValid(character))
	{
		return TArray<FHitResult>();
	}
	
	FVector startLoco = socketName == "None" ? character->GetActorLocation() : MeshComponent->GetSocketLocation(socketName); //if socket name has been passed then add it in location otherwise use the characters location
	
	//adding the collisions to look for 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	/*ObjectTypes.Reserve(1);*/
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> hits;

	//sphere trace collision
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		character->GetWorld(), /*we need to get the world through the character otherwise this cast doesnt work*/
		startLoco,
		startLoco,
		radius,
		ObjectTypes,
		false,
		ActorsHitToIgnore,
		EDrawDebugTrace::None,
		hits,
		true);

	if (!bHit)
	{
		return TArray<FHitResult>();
	}

	for (FHitResult hit : hits)
	{
		ActorsHitToIgnore.AddUnique(hit.GetActor());	
	}
	

	return hits;
}

bool UAnimNotifyState_HitDetection::DidAbilityCollide()
{
	return ActorsHitToIgnore.Num() - 1 > 0; /*the minus accounts for the player so its only looking for enemies that have been added*/
}

void UAnimNotifyState_HitDetection::ModifyStreak()
{
	if (!IsValid(character)) return;
	
	//did we hit or miss?
	if (DidAbilityCollide())
	{
		character->CurrentCombo->AddHitStreak(1);
	}
	else
	{
		character->CurrentCombo->ClearStreak();
		character->ResetCombo();
	}
}

void UAnimNotifyState_HitDetection::CleanUp()
{
	//reset the actors to ignore to nothing
	ActorsHitToIgnore.Empty();
}
