// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifyState_HitDetection.h"

#include "Chaos/PBDSuspensionConstraintData.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_HitDetection::Init(USkeletalMeshComponent* MeshComponent)
{
	if (!IsValid(MeshComponent))
	{
		return;
	}
	
	character = Cast<AGladiatorBaseChar>(MeshComponent->GetOwner()); //set the global character var here
	
	ActorsHitToIgnore.AddUnique(character);

	if (IsRunningGame()) prevLoco = SocketToAttachTo == "None" ? character->GetActorLocation() : MeshComponent->GetSocketLocation(SocketToAttachTo);
} 

TArray<FHitResult> UAnimNotifyState_HitDetection::GenerateTraceCollision(USkeletalMeshComponent* MeshComponent, float radius, int subSteps, FName socketName, ETraceType traceMode, EDrawDebugTrace::Type drawType)
{
	if (!IsValid(MeshComponent) && !IsValid(character))
	{
		return TArray<FHitResult>();
	}

	if (!MeshComponent->GetAnimInstance()->Montage_IsPlaying(NULL))
	{
		return TArray<FHitResult>();
	}
	TArray<FHitResult> hitResults;
	switch (traceMode)
	{
		case ETraceType::Continuous:
			 hitResults = FrameIndependentDetection(MeshComponent, radius, socketName, subSteps, drawType);
			break;
	case ETraceType::Discrete:
		hitResults = FrameDependentDetection(MeshComponent, radius, socketName, drawType);
		break;
	}
	
	//

	return hitResults;
}

TArray<FHitResult> UAnimNotifyState_HitDetection::FrameDependentDetection(USkeletalMeshComponent* MeshComponent, float radius,
	FName socketName, EDrawDebugTrace::Type drawType)
{
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
		drawType,
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
	
	if (hits.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The amount of hits was: %d"), hits.Num());
	}
	
	return hits;
}

TArray<FHitResult> UAnimNotifyState_HitDetection::FrameIndependentDetection(USkeletalMeshComponent* MeshComponent,
	float radius, FName socketName, int substeps = 1, EDrawDebugTrace::Type drawType = EDrawDebugTrace::None)
{
	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActors(ActorsHitToIgnore); //set ignored actors
	
	FVector currentLoco = socketName == "None" ? character->GetActorLocation() : MeshComponent->GetSocketLocation(socketName);

	/*currentLoco += currentLoco.RightVector * 10;*/ 
	
	FCollisionShape CollisionShapeType = FCollisionShape::MakeSphere(radius);
	
	TArray<FHitResult> hitResultsT;
	
	TArray<FHitResult> hits;

	FVector rotationDirection = currentLoco - prevLoco; 
	FRotator rot = rotationDirection.Rotation();
	FQuat rotQuat = rot.Quaternion();
	bool bHit;
	//substeping - calculate what each segment will be 
	FVector segment = (currentLoco - prevLoco) / substeps;

	for (int i = 0; i < substeps; ++i)
	{
		//calculate the start loco and endloco from the prevLoco  
		FVector startLoco = prevLoco + segment * i; //getting the start location
		FVector EndLoco = prevLoco + segment * (i + 1);

		/*sweep that area*/
		bHit = character->GetWorld()->SweepMultiByObjectType(
		hits, startLoco, EndLoco,
		FQuat::Identity,
		objectParams,
		CollisionShapeType,
		collisionParams);

		if (drawType != EDrawDebugTrace::None)
		{
			DrawDebugSphere(character->GetWorld(), currentLoco, radius, 20, (!bHit ? FColor::Red : FColor::Green), false, 2.f);
		}

		//if we hit something 
		for (FHitResult hit : hits)
		{
			ActorsHitToIgnore.AddUnique(hit.GetActor());

			/*bool bAlreadyExists = hitResultsT.ContainsByPredicate([&](const FHitResult& other)
			{
				return other.GetActor() == hit.GetActor();
			});
			if (!bAlreadyExists)
			{
				hitResultsT.Add(hit);
			}*/
		}
		
		
	}
	
	

	prevLoco = currentLoco; //set the prev to the currentloco at the end 
	UE_LOG(LogTemp, Warning, TEXT("new prevLoco: %f %f %f"), prevLoco.X, prevLoco.Y, prevLoco.Z);

	
	
	if (!bHit)
	{
		return TArray<FHitResult>();
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
