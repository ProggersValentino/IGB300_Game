// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GladiatorPlayerChar.h"

#include "FrameTypes.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IGB300_Geme/EnemyBase.h"
#include "EnemySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/GladiatorPlayerController.h"
#include "Player/GladiatorPlayerState.h"
#include "UI/GladiatorHUDBase.h"
#include <cstddef>

#include "MovieSceneTracksComponentTypes.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AGladiatorPlayerChar::AGladiatorPlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	KillCameraPosition = CreateDefaultSubobject<UGladiatorCameraPositionComponent>(TEXT("CameraPosition"));
	

	
	
	//KillCameraPosition->SetupAttachment(RootComponent);
	/*KillCameraPosition->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);*/

}

void AGladiatorPlayerChar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); //call to parent implementation

	InitAbilitySystemComp();
	
	
	if (!AbilitySystemComponent) return;

	GiveDefaultAbilities();
	GiveAndActivateIdleAbilities();
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

	gCurrentPlayerSpeed = GetCharacterMovement()->Velocity.Size();

	//FRotator cameraRot = CameraManager->GetCameraRotation();
	
	
	
	if (bUseCameraLerpWithMouse)
	{
		LerpCameraSystem(mouseInput);
	}

	UpdateMovementCameraShake(gCurrentPlayerSpeed);
	
	mouseInput = FVector2D::ZeroVector; 
	
}

void AGladiatorPlayerChar::SetYawSensivity(float sensValue)
{
	yawSensivity = sensValue;
}

void AGladiatorPlayerChar::SetPitchSensivity(float sensValue)
{
	pitchSensivity = sensValue;
}

float AGladiatorPlayerChar::GetYawSensivity()
{
	return yawSensivity;
}

float AGladiatorPlayerChar::GetPitchSensivity()
{
	return pitchSensivity;
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
	
	//binding speed attribute changes to the pawn's speed setting
	AGladiatorPlayerState* playerState = Cast<AGladiatorPlayerState>(GetPlayerState());
	UGladiatorAttributeSet* ATS = playerState->GetAttributeSet();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ATS->GetSpeedAttribute()).AddUObject(this, &AGladiatorBaseChar::OnSpeedChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &AGladiatorPlayerChar::OnHealthChanged);

	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Gameplay.State.Healthy"));
	
	GetCharacterMovement()->MaxWalkSpeed = ATS->GetSpeedAttribute().GetNumericValue(ATS);

	//retrieving camera manager for use
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	CameraComponent = GetComponentByClass<UCameraComponent>();
	CreateAndApplyDynamicMaterialToCamera();

	EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();	
	
	Maestro = Cast<AMaestroBase>(UGameplayStatics::GetActorOfClass(GetWorld(), maestroClass));

	InitInputBuffer();


	playerSpringArmComponent = GetComponentByClass<USpringArmComponent>();

	CameraDefaultPos = CameraComponent->GetRelativeLocation();
	
}

void AGladiatorPlayerChar::Die()
{
	Super::Die();
	Maestro->AchieveRageBaiter();
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

	/*we only want the player model to follow the direction of the camera if the player is moving
	 */
	if (gCurrentPlayerSpeed > 2.0f || isAttacking)
	{
		LerpPlayerRotation(PlayerLerpTime);		
	}
	
	//lock on target system 
	if (IsValid(CurrentLockedTarget) && bCanBeLocked)
	{
		if (!IsInViewOfTarget(CurrentLockedTarget)) //if we arent in view of the target disconnect the lock on 
		{
			ClearLockOn();
			return;
		}
		
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
	if (!bIsLerping)
	{
		return; //if we arent lerping just return}
	}
	if (CameraOverTime < time)
	{
		CameraOverTime += GetWorld()->GetDeltaSeconds();
		
		float alpha = FMath::Clamp((CameraOverTime / time) * AlphaSharpness, 0.f, 1.f); //ensures the value will be between 0 & 1
		float dragCalculation = DetermineDragCalculation(CameraDragSettings, alpha); //determines what calculation of drag we will apply to the lerp which is set in the CameraDragSettings

		//lerping betweeen the current camera rot to the new rotation that the player wants to look at. this happens overtime in the tick() method
		smoothYawInput = FMath::Lerp(smoothYawInput, values.X, dragCalculation * yawSensivity);
		smoothPitchInput = FMath::Lerp(smoothPitchInput, values.Y, dragCalculation * pitchSensivity);

		AGladiatorPlayerController* cons = Cast<AGladiatorPlayerController>(GetController());
		FRotator cameraRot = cons->GetControlRotation();

		

		
		
		//once lerp is calculated we are able to apply that to the camera to follow the player if it set to unlocked in ECameraLockSettings
		if (CameraYawAxisSettings == ECameraLockSettings::Unlocked)
		{
			AddControllerYawInput(smoothYawInput);
			float desiredPitch = cons->GetControlRotation().Pitch;

			//calculation for the max and min pitch allowed
			float minPitch = 0.f - CameraRotationPitchMinClamp < 0.f ? 360.f - CameraRotationPitchMinClamp : 0.f - CameraRotationPitchMinClamp;
			float maxPitch = 0.f + CameraRotationPitchMaxClamp;

			UE_LOG(LogTemp, Warning, TEXT("camera rotation: %f"), desiredPitch);

			if (desiredPitch < minPitch && desiredPitch > 180.f) cons->SetControlRotation(FRotator(minPitch, cameraRot.Yaw, cameraRot.Roll));
			else if (desiredPitch > maxPitch && desiredPitch < 180.f) cons->SetControlRotation(FRotator(maxPitch, cameraRot.Yaw, cameraRot.Roll));

		}
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
		
		float alpha = FMath::Clamp((TargetOverTime / time) * AlphaSharpness, 0.f, 1.f); //ensures the value will be between 0 & 1
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
	if (!bIsPlayerLerping && bPlayerIsSuckingToTarget) return;
	
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
	ActorsToIgnore.Add(this); //adding the player to ensure it doesnt hit itself

	//grabbing cam to gets location
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

AEnemyBase* AGladiatorPlayerChar::CycleToNextTarget(TArray<FHitResult> enemies)
{
	if (enemies.Num() == 0) return NULL;
	
	
	iterator++;
	if (iterator > enemies.Num() - 1) iterator = 0;

	AActor* hitActor = enemies[iterator].GetActor();
	
	//if (!IsValid(hitActor)) return; //test to see if actor is still active in scene otherwise return*/
	
	AEnemyBase* target = Cast<AEnemyBase>(hitActor);
	if (target)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                         // Key (-1 = add new, or use ID to overwrite)
			5.0f,                       // Duration (seconds)
			FColor::Green,             // Text color
			 FString::Printf(TEXT("iterator: %d"), iterator)    // Message
		);
		SetLockedTarget(target);	
	}
	return target;

	
}

bool AGladiatorPlayerChar::IsInViewOfTarget(AEnemyBase* Target)
{
	FVector forwardVec = GetActorForwardVector();

	FVector difference = Target->GetActorLocation() - GetActorLocation();

	float dotDifference = FVector::DotProduct(forwardVec, difference);

	float HalfAngleRad = FMath::DegreesToRadians(LookZoneBeforeDeactivate) * 0.5f;

	float CosHalfAngle = FMath::Cos(HalfAngleRad);
	
	return dotDifference > CosHalfAngle;
}

// Returns a reference to the previously locked on target for use in BP
AEnemyBase* AGladiatorPlayerChar::ClearLockOn()
{
	AEnemyBase* oldTarget = CurrentLockedTarget;
	EnemyDehighlight(oldTarget);
	CurrentLockedTarget = nullptr;
	if (IsValid(currentDecal)) currentDecal->DestroyComponent();
	return oldTarget;
}

void AGladiatorPlayerChar::EnemyDehighlight(AEnemyBase* Enemy)
{
	if (Enemy == NULL)
	{
		return;
	}
	Enemy->GetMesh()->SetCustomDepthStencilValue(0);
}

void AGladiatorPlayerChar::PredictEnemyDeath(float searchRadius, EDrawDebugTrace::Type Debug, float debugTraceTime)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesAllowed;
	ObjectTypesAllowed.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult hitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); //adding the player to ensure it doesnt hit itself

	/*GEngine->AddOnScreenDebugMessage(
			-1,                         // Key (-1 = add new, or use ID to overwrite)
			5.0f,                       // Duration (seconds)
			FColor::Green,             // Text color
			 FString::Printf(TEXT("Player Speed: %f"), currentPlayerSpeed)    // Message
		);*/
	
	//grabbing cam to gets location
	UCameraComponent* cam = FindComponentByClass<UCameraComponent>();
	FVector StartLoco = cam->GetComponentLocation();
	FVector EndLoco = cam->GetComponentLocation() + cam->GetForwardVector() * 250;
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoco, EndLoco, searchRadius, ObjectTypesAllowed, false, ActorsToIgnore, Debug, hitResult, true,
		FLinearColor::Red, FLinearColor::Green, debugTraceTime);

	if (!bHit)
	{
		return;
	}

	AGladiatorBaseChar* enemy = Cast<AGladiatorBaseChar>(hitResult.GetActor());

	float enemyHealth = enemy->GetAttributeSet()->GetHealthAttribute().GetNumericValue(enemy->GetAttributeSet());
	float playerDmg =  enemy->GetAttributeSet()->GetMitigatedDamage(AttributeSet->GetBaseDamage());
	
	float predictCalc = enemyHealth - playerDmg;

	bool isGoingToDieNextHit = predictCalc <= 0;

	bool lastEnemy = EnemySubsystem->GetEnemyCount() == 0;

	int currentWave =  Maestro->GetCurrentWave();
	int NumWaves = Maestro->GetNumWaves() - 1; //wave gets increased after last enemy gets defeated so we need to be a little early
	
	bool isLastWave = currentWave >= NumWaves;
	
	if (isGoingToDieNextHit && lastEnemy && isLastWave)
	{
		//activate camera shift
		TransistionCameraTargetView(FinalKillCamera);
	}
}


void AGladiatorPlayerChar::TransistionCameraTargetView(TSubclassOf<AGladiatorCameraBase> Target)
{
	AGladiatorPlayerController* playercon = Cast<AGladiatorPlayerController>(GetController());

	AActor* spawnedCam = GetWorld()->SpawnActor(Target);

	FVector locao = KillCameraPosition->GetComponentLocation();
	
	spawnedCam->SetActorLocation(locao);
	
	/*UChildActorComponent* camera = NewObject<UChildActorComponent>(spawnedCam);
	camera->SetupAttachment(KillCameraPosition);
	camera->SetChildActorClass(FinalKillCamera);*/
	
	playercon->SetViewTargetWithBlend(spawnedCam, .25f, VTBlend_Cubic);

	SpawnedKillCamera = spawnedCam;
}

void AGladiatorPlayerChar::TransitionBackToMainCamera()
{
	AGladiatorPlayerController* playercon = Cast<AGladiatorPlayerController>(GetController());
	playercon->SetViewTargetWithBlend(this, 1.f, VTBlend_Cubic);

	SpawnedKillCamera = nullptr;
}

void AGladiatorPlayerChar::TryActivateAttackType()
{

	if (inputBuffer[0].BufferAction == nullptr || AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Gameplay.State.IsPunching"))) //if the first element is null or we dont want to accept anymore inputs then just return
	{
		return;
	}

	ActivatePreAttackAdjustment();
	
	SelectAttackToUse(inputBuffer[0]);

	//pop the first array element by reshifting the entire array up one -> we have used an input 
	for (int i = 0; i < inputBuffer.Num(); i++)
	{
		int nextBuff = i + 1;

		if (nextBuff > inputBuffer.Num() - 1)
		{
			inputBuffer[i] = FInputBuffer();
		}
		else
		{
			inputBuffer[i] = inputBuffer[i + 1];
		}
		
	}
}



void AGladiatorPlayerChar::AddToBuffer(FInputBuffer bufferToInsert)
{
	if (inputBuffer[inputBuffer.Max() - 1].BufferAction || !bCanAcceptInputQueue) //we will not accept this buffer as the main buffer list is full 
	{
		return;
	}

	bufferToInsert.currentOwnedTags = AbilitySystemComponent->GetOwnedGameplayTags();
	
	for (int i = 0; i < inputBuffer.Num(); i++)
	{
		if (inputBuffer[i].BufferAction == nullptr)
		{
			inputBuffer[i] = bufferToInsert;
			break;
		}
	}
}

void AGladiatorPlayerChar::DisableInputQueuing()
{
	bCanAcceptInputQueue = false;
}

void AGladiatorPlayerChar::EngageInputQueuing()
{
	bCanAcceptInputQueue = true;
}

EAttackHoldStage AGladiatorPlayerChar::DetermineCurrentHoldStage(float timeHeld)
{
	if (timeHeld  >= tapAttackTime && timeHeld < longHoldAttackTime)
	{
		return EAttackHoldStage::Tap;
	}
	else if (timeHeld >= longHoldAttackTime && timeHeld < longHoldAttackTime)
	{
		return EAttackHoldStage::MediumHold;
	}
	else
	{
		return EAttackHoldStage::LongHold;
	}
}

void AGladiatorPlayerChar::ActivateHoldStage(EAttackHoldStage stage)
{

	if (previousStage == stage)
	{
		return;
	}
	
	switch (stage)
	{
		case EAttackHoldStage::Tap:
			//windUpTimeSpeed = 0.09f / mediumAttackHeldTime; //the 0.09 is a hard coded value for how long the animation takes so using this value we can extend how long it will taake to complete by giving the length 
			return;

		case EAttackHoldStage::MediumHold:
			//activate camera shake
			UpdateCameraShake(mediumHoldStageBuildUp, currentActiveAttackHoldCameraShake);
			break;
		case EAttackHoldStage::LongHold:
			UpdateCameraShake(longHoldStageBuildUp, currentActiveAttackHoldCameraShake);
			//windUpTimeSpeed = 0.5f;
			break;
		
		default:
			UE_LOG(LogTemp, Error, TEXT("Failed to read attack hold stage in ActivateHoldStage()"))
			break;
	}

	previousStage = stage;
}

void AGladiatorPlayerChar::CompleteHoldStage()
{
	ClearCameraShake(currentActiveAttackHoldCameraShake);
}

void AGladiatorPlayerChar::AdjustCameraZoom(float zoomValue, float time)
{
	cameraChangeAlpha = 0.f;

	FVector goal = CameraComponent->GetRelativeLocation().ForwardVector * zoomValue;
	FVector start = CameraComponent->GetRelativeLocation();
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AGladiatorPlayerChar::ApplyZoom, start, goal, time);

	GetWorld()->GetTimerManager().ClearTimer(CameraZoomTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CameraZoomTimerHandle, timerDelegate, 0.005f, true);
}

void AGladiatorPlayerChar::ResetToDefaultZoom(float time)
{
	cameraChangeAlpha = 0.f;
	FVector target = CameraDefaultPos;
	FVector start = CameraComponent->GetRelativeLocation();
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AGladiatorPlayerChar::ApplyZoom, start, target, time);

	GetWorld()->GetTimerManager().ClearTimer(CameraZoomTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CameraZoomTimerHandle, timerDelegate, 0.01f, true);
}

void AGladiatorPlayerChar::ApplyZoom(FVector startingLoco, FVector zoomValue, float time)
{
	cameraChangeAlpha += GetWorld()->GetDeltaSeconds();
	float rawAlpha = FMath::Clamp( cameraChangeAlpha / time, 0.f, 1.f);
	float alpha = FMath::InterpEaseOut(0.f, 1.f, rawAlpha, 2.f);
	
	FVector percentZoom = FMath::Lerp(startingLoco, zoomValue, alpha);

	/*FVector newLoco = percentZoom + currentLoco;*/
	
	CameraComponent->SetRelativeLocation(percentZoom);

	if (cameraChangeAlpha > 0.95f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraZoomTimerHandle);
	}
	
	//cameraChangeAlpha += GetWorld()->GetDeltaSeconds();
	
}

void AGladiatorPlayerChar::ActivatePreAttackAdjustment_Implementation()
{
}

void AGladiatorPlayerChar::UpdateMovementCameraShake(float speed)
{
	
	if (speed > 2.0f && !isRunning)
	{
		isRunning = true;
		UpdateCameraShake(RunShake, currentActiveCameraShake);
	}
	else if (speed < 2.0f && isRunning)
	{
		isRunning = false;
		UpdateCameraShake(IdleShake, currentActiveCameraShake);
	}
}

void AGladiatorPlayerChar::UpdateCameraShake(TSubclassOf<UCameraShakeBase> shakeToActivate, UCameraShakeBase*& shakeValue)
{
	if (shakeValue && shakeValue->IsActive())
	{
		CameraManager->StopCameraShake(shakeValue);
	}

	shakeValue = CameraManager->StartCameraShake(shakeToActivate);
}

void AGladiatorPlayerChar::ClearCameraShake(UCameraShakeBase*& shakeValue)
{
	if (shakeValue && shakeValue->IsActive())
	{
		CameraManager->StopCameraShake(shakeValue);	
	}
}


void AGladiatorPlayerChar::AddAttackToMemory(EAttackType type)
{
	//shift the attacks down 1 leaving the top free
	for (int i = prevExecutedAttacks.Num() - 1; i > 0; i--)
	{
		if (i - 1 < 0)
		{
			break;
		}
		
		prevExecutedAttacks[i] = prevExecutedAttacks[i - 1];
	}

	prevExecutedAttacks[0] = type; //add in the type
}

void AGladiatorPlayerChar::CreateAndApplyDynamicMaterialToCamera()
{
	FWeightedBlendables blendies = CameraComponent->PostProcessSettings.WeightedBlendables;

	//this is a must as we need the second material instance as it is the Enemy Stencile
	if (blendies.Array.Num() < 2)
	{
		return;
	}
	
	PPDamagedMat = Cast<UMaterialInstanceDynamic>(blendies.Array[1].Object); //this code susceptiable to a simple order change in the Post Process volumn box 

	if (!PPDamagedMat)
	{
		return;
	}
	

}



void AGladiatorPlayerChar::ApplyEffect(float timeToLerp)
{
	float percent = FMath::Lerp(timeToLerp, 0, timerAlpha * GetWorld()->GetDeltaSeconds() * 125);

	PPDamagedMat->SetScalarParameterValue("DesaturationStrength", percent);

	UE_LOG(LogTemp, Error, TEXT("current combo index: %f"), percent);

	//once we have lerped to our destination stop the timer
	if (percent < 0.05f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		PPDamagedMat->SetScalarParameterValue("DesaturationStrength", 0.0);
	}

	timerAlpha += GetWorld()->GetDeltaSeconds();
	
	
}

void AGladiatorPlayerChar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	timerAlpha = 0.f;
	
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AGladiatorPlayerChar::ApplyEffect, 2.f);
	
	if (!PPDamagedMat)
	{
		return;
	}

	float difference = Data.NewValue - Data.OldValue;
	
	if (difference < 0)
	{
		PPDamagedMat->SetScalarParameterValue("DesaturationStrength", 1.5f);
	
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, 0.01f, true);

		//to prevent the hit buffer from storing data for a long period of time
		GetWorld()->GetTimerManager().ClearTimer(HitBufferHandle);
		GetWorld()->GetTimerManager().SetTimer(HitBufferHandle, this, &AGladiatorPlayerChar::ClearHitBuffer, timeTillHitBufferExpires);

		TryActivateIframess();
		
	}

	if (HealthStateTag.IsValid())
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(HealthStateTag); //remove the previous current tag
	}
	
	HealthStateTag = GetHealthStateTag(Data.NewValue, GetAttributeSet()->GetMaxHealth());
	AbilitySystemComponent->AddLooseGameplayTag(HealthStateTag); //apply new one

	
	if (HealthStateTag.GetTagName() == "Gameplay.State.Critical")
	{
		PPDamagedMat->SetScalarParameterValue("CriticalHaloStrength", 1.5f);
	}
	else
	{
		PPDamagedMat->SetScalarParameterValue("CriticalHaloStrength", 0.f);
	}
	
	
}

void AGladiatorPlayerChar::InitInputBuffer()
{
	inputBuffer.Reserve(3);
	prevExecutedAttacks.Reserve(3);
	
	for (int i = 0; i < inputBuffer.Max(); i++)
	{
		inputBuffer.Add(FInputBuffer());
	}

	for (int i = 0; i < prevExecutedAttacks.Max(); i++)
	{
		prevExecutedAttacks.Add(EAttackType::None);
	}
}

void AGladiatorPlayerChar::ClearAttacksMemory()
{
	for (int i = 0; i < prevExecutedAttacks.Num(); i++)
	{
		prevExecutedAttacks[i] = EAttackType::None;
	}
}

void AGladiatorPlayerChar::SelectAttackToUse(FInputBuffer selectedBuffer)
{
	GetWorld()->GetTimerManager().ClearTimer(combatTimerHandle);
	
	float blockedRage = AttributeSet->GetBlockedRageAttribute().GetNumericValue(AttributeSet);
	float maxBlockedRage = AttributeSet->GetMaxBlockedRageAttribute().GetNumericValue(AttributeSet);

	bool bLastAttackWasMedium = prevExecutedAttacks[0] == EAttackType::Medium && prevExecutedAttacks[1] != EAttackType::Medium; //only allow if the prev attack was medium and not twice in row
	bool bLastAttackWasFollowup = prevExecutedAttacks[0] == EAttackType::FollowUp;
	
	//if we are blocking and our rage is at max then activate utility ability	
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Gameplay.Ability.Block")) && blockedRage >= maxBlockedRage)
	{
		ActivateCombo(EAttackType::Utility);
		AddAttackToMemory(EAttackType::Utility);
	}
	else
	{
		//determine the attack type based off how long the button was held for
		if (selectedBuffer.inputHeldTime >= tapAttackTime && inputBuffer[0].inputHeldTime < longHoldAttackTime && !bLastAttackWasMedium) //light attack 
		{
			ActivateCombo(EAttackType::Light);
			AddAttackToMemory(EAttackType::Light);
		}
		else if (selectedBuffer.inputHeldTime >= tapAttackTime && inputBuffer[0].inputHeldTime < longHoldAttackTime && bLastAttackWasMedium) //medium follow up
		{
			ActivateCombo(EAttackType::FollowUp);
			AddAttackToMemory(EAttackType::FollowUp);
		}
		else if (selectedBuffer.inputHeldTime >= longHoldAttackTime && !bLastAttackWasMedium && !bLastAttackWasFollowup) //medium attack
		{
			ActivateCombo(EAttackType::Medium);
			AddAttackToMemory(EAttackType::Medium);
		}
		else if(selectedBuffer.inputHeldTime >= longHoldAttackTime && (bLastAttackWasMedium || bLastAttackWasFollowup)) //heavy attack
		{
			ActivateCombo(EAttackType::Heavy);
			AddAttackToMemory(EAttackType::Heavy);
		}	
	}

	//activate timer to wipe the stored attacks in prevExecuteAttack array to prevent players from stocking a medium attack tap for later 
	GetWorld()->GetTimerManager().SetTimer(combatTimerHandle, this, &AGladiatorPlayerChar::ClearAttacksMemory, timeTillAttackMemoryWiped, false);

}

void AGladiatorPlayerChar::TryActivateIframess()
{
	if (HitBuffer < amountOfTimesHitTolerance)
	{
		return;
	}
	
	FGameplayAbilitySpec AbilitySpec(IFramesAbiltiy, 1); //data surrounding for the ability class

	AbilitySystemComponent->GiveAbilityAndActivateOnce(AbilitySpec);

	ClearHitBuffer();
}

void AGladiatorPlayerChar::ClearHitBuffer()
{
	HitBuffer = 0;
}

void AGladiatorPlayerChar::EnemyHighlight(AEnemyBase* Enemy)
{
	if (Enemy == NULL)
	{
		return;
	}
	Enemy->GetMesh()->SetCustomDepthStencilValue(1);
}

FHitResult AGladiatorPlayerChar::DetectEnemyToSuckTo(float Radius, EDrawDebugTrace::Type Debug, float debugTraceTime)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesAllowed;
	ObjectTypesAllowed.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult hitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); //adding the player to ensure it doesnt hit itself
	
	
	float currentSpeed = GetCharacterMovement()->Velocity.Size();

	FVector StartLoco = FVector::ZeroVector;
	FVector EndLoco = FVector::ZeroVector;
	
	
	//are we going forward or backward? apply the necessary math to suit each case
	FVector2d STTDirectionMultiplier = FVector2d::ZeroVector;


	if (InputActionValue.X > 0.02 || InputActionValue.X < -0.02)
	{
		STTDirectionMultiplier.X = InputActionValue.X * currentSpeed;
	}

	if (InputActionValue.Y > 0.02 || InputActionValue.Y < -0.02)
	{
		STTDirectionMultiplier.Y = InputActionValue.Y > 0 ?
			(currentSpeed * InputActionValue.Y) * ForwardSTTMultiplier:
			(currentSpeed * InputActionValue.Y) * BackwardSTTMultiplier;
	}

	GEngine->AddOnScreenDebugMessage(
			-1,                         // Key (-1 = add new, or use ID to overwrite)
			5.0f,                       // Duration (seconds)
			FColor::Green,             // Text color
			 FString::Printf(TEXT("Player Directional: %f, %f "), STTDirectionMultiplier.X, STTDirectionMultiplier.Y)    // Message
		);

	
	if (currentSpeed > 0.2f)
	{
		FVector newPosVec = FVector(1.f, 1.f, 0.f);

		UCameraComponent* cam = FindComponentByClass<UCameraComponent>();
		
		//rotation calculation where the player actor should be looking at (which where the camera is looking) 
		FVector camDirFor = cam->GetForwardVector();
		camDirFor.Z = 0.f;
		camDirFor.Normalize();
		FVector camDirRig = cam->GetRightVector();
		camDirRig.Z = 0.f;
		camDirRig.Normalize();
		
		StartLoco = GetActorLocation();

		float forwardValue = STTDirectionMultiplier.Y;
		float rightValue = STTDirectionMultiplier.X;
		
		FVector desiredDirection = (camDirFor * STTDirectionMultiplier.Y) + (rightValue * camDirRig);
		
		EndLoco = (StartLoco + desiredDirection); 
	}
	else
	{
		STTDirectionMultiplier.Y = NeutralSTTMultiplier;
		
		//grabbing cam to gets location
		UCameraComponent* cam = FindComponentByClass<UCameraComponent>();
		StartLoco = GetActorLocation();
		EndLoco = StartLoco + cam->GetForwardVector() * STTDirectionMultiplier.Y;
	}
	
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoco, EndLoco, Radius, ObjectTypesAllowed, false, ActorsToIgnore, Debug, hitResult, true,
		FLinearColor::Red, FLinearColor::Green, debugTraceTime);

	if (!bHit)
	{
		return hitResult;
	}

	//rotate the player to face the target 
	FRotator newplayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hitResult.GetActor()->GetActorLocation());
		
	SetActorRotation(FRotator(0, newplayerRot.Yaw, 0));
	
	return hitResult;
}





