// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerChar.h"

#include "AsyncTreeDifferences.h"
#include "FrameTypes.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractiveToolManager.h"
#include "Camera/CameraComponent.h"
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
	
	bIsLerping = true;
	bIsPlayerLerping = true;
	CameraOverTime = 0.0f;
	
}

void AGladiatorPlayerChar::LerpCameraSystem(const FVector2D values)
{
	LerpInput(mouseInput, CameraLerpTime);
	LerpPlayerRotation(mouseInput, PlayerLerpTime);
	
}

void AGladiatorPlayerChar::LerpInput(const FVector2D values, float time)
{
	if (!bIsLerping) return;
	
	if (CameraOverTime < time)
	{
		CameraOverTime += GetWorld()->GetDeltaSeconds();
		
		float alpha = FMath::Clamp(CameraOverTime / time, 0.f, 1.f);

		float dragCalculation = DetermineDragCalculation(CameraDragSettings, alpha);
		
		smoothYawInput = FMath::Lerp(smoothYawInput, values.X, dragCalculation * sensivity);
		//FVector2D smoothYawInput = FMath::Vector2DInterpTo(smoothYawInput, values, GetWorld()->GetDeltaSeconds(), 20.0f * 20);
		
		AddControllerYawInput(smoothYawInput);
		//AddControllerPitchInput(smoothYawInput.Y);
	
	}
	else
	{
		bIsLerping = false;
		CameraOverTime = 0;
	}
		
	//AddControllerYawInput(yaw);
}

void AGladiatorPlayerChar::LerpPlayerRotation(const FVector2D values, float time)
{
	if (!bIsPlayerLerping) return;
	
	if (PlayerOverTime < time)
	{
		PlayerOverTime += GetWorld()->GetDeltaSeconds();
		
		double direction = atan2(values.Y, values.X);

		double magnitude = sqrt(values.X * values.X + values.Y * values.Y);
		
		float yaw = magnitude * cos(direction) * 2;
		float pitch = magnitude * sin(direction) * 2;
		
		float alpha = FMath::Clamp(PlayerOverTime / time, 0.f, 1.f);

		UCameraComponent* cam = FindComponentByClass<UCameraComponent>();
        
		cameraDirection = cam->GetComponentLocation() + (cam->GetForwardVector() * 2000);
        
		FVector lookDirection = cameraDirection - GetActorLocation();

		lookDirection.RotateAngleAxis(60, FVector(0.0f, 0.0f, 1.0f));

		float dragCalculation = DetermineDragCalculation(PlayerCharacterDragSettings, alpha);
		
		FRotator newRot = (FRotator)FQuat::Slerp(GetActorRotation().Quaternion(), lookDirection.Rotation().Quaternion(), dragCalculation);
		//FRotator newRot = FMath::RInterpTo(GetActorRotation(), future, GetWorld()->GetDeltaSeconds(), 5.f);
		
		SetActorRotation(newRot);
	}
	else
	{
		bIsPlayerLerping = false;
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




