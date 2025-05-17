// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerChar.h"

#include "FrameTypes.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "IGB300_Geme/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/GladiatorPlayerController.h"
#include "Player/GladiatorPlayerState.h"
#include "UI/GladiatorHUDBase.h"

// Sets default values
AGladiatorPlayerChar::AGladiatorPlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGladiatorPlayerChar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); //call to parent implementation

	InitAbilitySystemComp();
	
	
	if (!AbilitySystemComponent) return;

	GiveDefaultAbilities();
	InitDefaultAttributes();
	InitHUD();
	
}

void AGladiatorPlayerChar::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComp();
	InitDefaultAttributes();
	InitHUD();
	
}

void AGladiatorPlayerChar::SetupPlayerInputComponent(UInputComponent* playerInput)
{
	Super::SetupPlayerInputComponent(playerInput);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent, UInputComponent>(playerInput);
	input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGladiatorPlayerChar::CameraInputCallback); //binding cameraInputCallback to the Look Input Action so when the player looks everytime, this method will trigger
}

void AGladiatorPlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LerpCameraSystem(mouseInput);

	mouseInput = FVector2D::ZeroVector;
	
}


/// Initializing the Ability system comp on the player character through extracting from the player state
void AGladiatorPlayerChar::InitAbilitySystemComp()
{
	AGladiatorPlayerState* playerState = GetPlayerState<AGladiatorPlayerState>(); //we initializing the AbilitySystemComp on the player state therefore we need to get access to it

	check(playerState); //can we safely dereference the pointer? 

	AbilitySystemComponent = CastChecked<UGladiatorAbilitySystemComponent>(
		playerState->GetAbilitySystemComponent());

	AbilitySystemComponent->InitAbilityActorInfo(playerState, this);

	AttributeSet = playerState->GetAttributeSet();

	FString Message = FString::Printf(TEXT("playerstate: %s"), *playerState->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
}

void AGladiatorPlayerChar::InitHUD() const
{
	if (const AGladiatorPlayerController* playerController = Cast<AGladiatorPlayerController>(GetController()))
	{
		if (AGladiatorHUDBase* GladHUD = Cast<AGladiatorHUDBase>(playerController->GetHUD()))
		{
			GladHUD->Init();
		}
	}
}

void AGladiatorPlayerChar::BeginPlay()
{
	Super::BeginPlay();
}

void AGladiatorPlayerChar::CameraInputCallback(const FInputActionInstance& instance)
{
	FVector2D input = instance.GetValue().Get<FVector2D>();

	mouseInput = input;
	TargetOverTime = 0.0f;
	
	bCanBeLocked = false;
	
	bIsLerping = true;
	bIsPlayerLerping = true;
	CameraOverTime = 0.0f;
	
}

void AGladiatorPlayerChar::LerpCameraSystem(const FVector2D values)
{
	LerpInput(mouseInput, CameraLerpTime);
	LerpPlayerRotation(PlayerLerpTime);	

	if (IsValid(CurrentLockedTarget) && bCanBeLocked)
	{
		bIsLocking = true;
		bIsPlayerLerping = true;
		LerpToTarget(CurrentLockedTarget, LerpTimeToTarget);
		LerpPlayerRotation(LerpTimeToTarget);
	}
	else
	{
		bIsLocking = false;
	}
	
}

void AGladiatorPlayerChar::LerpInput(const FVector2D values, float time)
{
	if (!bIsLerping) return; //if we arent lerping just return
	
	if (CameraOverTime < time)
	{
		CameraOverTime += GetWorld()->GetDeltaSeconds();
		
		float alpha = FMath::Clamp(CameraOverTime / time, 0.f, 1.f); //ensures the value will be between 0 & 1
		float dragCalculation = DetermineDragCalculation(CameraDragSettings, alpha); //determines what calculation of drag we will apply to the lerp which is set in the CameraDragSettings

		//lerping betweeen the current camera rot to the new rotation that the player wants to look at. this happens overtime in the tick() method
		smoothYawInput = FMath::Lerp(smoothYawInput, values.X, dragCalculation * sensivity);
		smoothPitchInput = FMath::Lerp(smoothPitchInput, values.Y, dragCalculation * sensivity);

		//once lerp is calculated we are able to apply that to the camera to follow the player if it set to unlocked in ECameraLockSettings
		if (CameraYawAxisSettings == ECameraLockSettings::Unlocked) AddControllerYawInput(smoothYawInput);
		if (CameraPitchAxisSettings == ECameraLockSettings::Unlocked) AddControllerPitchInput(smoothPitchInput);
		
	
	}
	else
	{
		bIsLerping = false;
		bCanBeLocked = true;
		CameraOverTime = 0;
	}
	
}

void AGladiatorPlayerChar::LerpToTarget(const AActor* target, float time)
{
	if (TargetOverTime < time)
	{
		TargetOverTime += GetWorld()->GetDeltaSeconds();
		
		float alpha = FMath::Clamp(TargetOverTime / time, 0.f, 1.f); //ensures the value will be between 0 & 1
		float dragCalculation = DetermineDragCalculation(LockOnDragSettings, alpha); //determines what calculation of drag we will apply to the lerp which is set in the CameraDragSettings

		FVector difference = target->GetActorLocation() - GetActorLocation();
		FRotator diffRot = difference.Rotation();

		FRotator newRot = (FRotator)FQuat::Slerp(GetControlRotation().Quaternion(), diffRot.Quaternion(), dragCalculation);

		GetController()->SetControlRotation(newRot);
	
	}
	else TargetOverTime = 0.f;

}

void AGladiatorPlayerChar::LerpPlayerRotation(float time)
{
	if (!bIsPlayerLerping) return;
	
	if (PlayerOverTime < time)
	{
		PlayerOverTime += GetWorld()->GetDeltaSeconds();
		
		float alpha = FMath::Clamp(PlayerOverTime / time, 0.f, 1.f); //ensures the value will be between 0 & 1

		UCameraComponent* cam = FindComponentByClass<UCameraComponent>(); //getting camera comp on the player

		//rotation calculation where the player actor should be looking at (which where the camera is looking) 
		cameraDirection = cam->GetComponentLocation() + (cam->GetForwardVector() * 2000);
		FVector lookDirection = cameraDirection - GetActorLocation();
		FRotator lookRotation = FRotator(0, lookDirection.Rotation().Yaw, 0);	//we only want the player rotation on the yaw
		
		//determines what calculation of drag we will apply to the slerp which is set in the PlayerCharacterDragSettings
		float dragCalculation = DetermineDragCalculation(PlayerCharacterDragSettings, alpha);

		//Slerping to the new rotation overtime with the tick() 
		FRotator newRot = (FRotator)FQuat::Slerp(GetActorRotation().Quaternion(), lookRotation.Quaternion(), dragCalculation);
		
		//apply the new rotation to the player to keep up with where the player is looking
		SetActorRotation(newRot);
	}
	else
	{
		bIsPlayerLerping = false;
		bCanBeLocked = true;
		PlayerOverTime = 0;
	}
}

float AGladiatorPlayerChar::DetermineDragCalculation(EDragSettings DragType, const float alpha)
{
	switch (DragType)
	{
		case EDragSettings::defaultDrag:
			return alpha;
		case EDragSettings::EaseInDrag:
			return alpha * alpha;
		case EDragSettings::EaseOutDrag:
			return 1.f - FMath::Pow(1.f - alpha, 2.f);
		case EDragSettings::EaseInOutDrag:
			return alpha * alpha * (3.0f - 2.0f * alpha);
	default:
		return 0.f;
	}
}

TArray<FHitResult> AGladiatorPlayerChar::GetEnemiesInView()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesAllowed;
	ObjectTypesAllowed.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> hitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	
	UCameraComponent* cam = FindComponentByClass<UCameraComponent>();
	FVector StartLoco = cam->GetComponentLocation();
	FVector EndLoco = cam->GetComponentLocation() + cam->GetForwardVector() * 2000;
	
	bool bHasHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartLoco, EndLoco, 400.f, ObjectTypesAllowed, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hitResult,
		true);

	iterator = -1; //reset iterator for the new array
	
	return hitResult;
}

void AGladiatorPlayerChar::SetLockedTarget(AEnemyBase* enemies)
{
	if (!IsValid(enemies))
	{
		ClearLockOn();
		return;
	}

	
	if (enemies != CurrentLockedTarget)
	{
		ClearLockOn();
		
		CurrentLockedTarget = enemies;

		//spawn lock on decal on target
		UDecalComponent* decal = UGameplayStatics::SpawnDecalAttached(LockOnDecal, LockOnDecalSize, CurrentLockedTarget->GetRootComponent(), "Name_NONE", 
		FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::KeepRelativeOffset, false);

		currentDecal = decal;	
	}
}

void AGladiatorPlayerChar::CycleToNextTarget(TArray<FHitResult> enemies)
{
	if (enemies.Num() == 0) return;
	
	iterator+=1;

	if (iterator >= enemies.Num() - 1) iterator = 0;
	
	AEnemyBase* target = CastChecked<AEnemyBase>(enemies[iterator].GetActor());

	GEngine->AddOnScreenDebugMessage(
		-1,                         // Key (-1 = add new, or use ID to overwrite)
		5.0f,                       // Duration (seconds)
		FColor::Green,             // Text color
		 FString::Printf(TEXT("iterator: %d"), iterator)    // Message
	);
	SetLockedTarget(target);
}

void AGladiatorPlayerChar::ClearLockOn()
{
	CurrentLockedTarget = nullptr;
	if (IsValid(currentDecal)) currentDecal->DestroyComponent();
}




