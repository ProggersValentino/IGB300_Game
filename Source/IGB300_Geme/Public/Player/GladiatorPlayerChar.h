// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySubsystem.h"
#include "GladiatorBaseChar.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Engine/DecalActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Camera/GladiatorCameraBase.h"
#include "Camera/GladiatorCameraPositionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameManagement/MaestroBase.h"
#include "GladiatorPlayerChar.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInputBuffer
{
	GENERATED_USTRUCT_BODY()

public:

	FInputBuffer() : BufferAction(nullptr), inputHeldTime(0.0f), currentOwnedTags()
	{
	}
	
	UPROPERTY(BlueprintReadWrite)
	UInputAction* BufferAction;

	UPROPERTY(BlueprintReadWrite)	
	float inputHeldTime;

	FGameplayTagContainer currentOwnedTags;
}; 

UENUM(BlueprintType)
enum class EAttackHoldStage : uint8
{
	Tap = 0,
	MediumHold = 1,
	LongHold = 2,
};


class AEnemyBase;

///determien what type drag you want to set the camera and the player
UENUM(BlueprintType)
enum class EDragSettings : uint8
{
	defaultDrag = 0 UMETA(DisplayName = "Default"),
	EaseInDrag = 1 UMETA(DisplayName = "EaseIn"),
	EaseOutDrag = 2 UMETA(DisplayName = "EaseOut"),
	EaseInOutDrag = 3 UMETA(DisplayName = "EaseInOut"),
};

UENUM(BlueprintType)
enum class ECameraLockSettings: uint8
{
	Unlocked = 0 UMETA(DisplayName = "Unlocked"),
	Locked = 1 UMETA(DisplayName = "Locked"),
};

UCLASS()
class IGB300_GEME_API AGladiatorPlayerChar : public AGladiatorBaseChar
{
	GENERATED_BODY()

public:
	void Die() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "Different drag behaviours", DisplayPriority = 0))
	EDragSettings CameraDragSettings; //different drag settings for camera
	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "Different drag behaviours", DisplayPriority = 0))
	EDragSettings PlayerCharacterDragSettings;

	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "Determines if the camera's yaw axis should lock and not move", DisplayPriority = 0))
	ECameraLockSettings CameraYawAxisSettings; //whether the camera yaw axis is locked
	
	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "Determines if the camera's pitch axis should lock and not move", DisplayPriority = 0))
	ECameraLockSettings CameraPitchAxisSettings; //whether the camera pitch axis is locked
	
	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "affects the speed of the camera rotation", DisplayPriority = 0, ClampMin = "0.1",
		ClampMax = "1.", UIMin = "0.1", UIMax = "1."))
	float CameraLerpTime = .15f; //affects the speed of the camera rotation

	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "affects the speed of the Player character's rotation",
		ClampMin = "0.1", ClampMax = "1.", UIMin = "0.1", UIMax = "2.", DisplayPriority = 0))
	float PlayerLerpTime = 2.f; //affects the speed of the player character's rotation

	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "mutliplier for the CameraLerpTime", DisplayPriority = 0, ClampMin = "0.1",
		ClampMax = "2.", UIMin = "0.5", UIMax = "2."))
	float yawSensivity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "mutliplier for the CameraLerpTime", DisplayPriority = 0, ClampMin = "0.1",
		ClampMax = "2.", UIMin = "0.5", UIMax = "2."))
	float pitchSensivity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gladiator Camera System", meta = (ToolTip = "how sharp do we want the aplha to be"))
	float AlphaSharpness = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gladiator Camera System", meta = (ToolTip = "If true, lerping with mouse input is enabled."))
	bool bUseCameraLerpWithMouse = true;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "Gladiator Camera System", meta =(ClampMin="0.0", ClampMax="90.0"))
	float CameraRotationPitchMaxClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "Gladiator Camera System", meta =(ClampMin="0.0", ClampMax="90.0"))
	float CameraRotationPitchMinClamp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Camera Shake", meta = (ToolTip = "When the player is idle then the camera will shake under these settings"))
	TSubclassOf<UCameraShakeBase> IdleShake;

	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Camera Shake", meta = (ToolTip = "When the player is moving then the camera will shake under these settings"))
	TSubclassOf<UCameraShakeBase> RunShake;

	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Camera Shake", meta = (ToolTip = "When the player is moving then the camera will shake under these settings"))
	TSubclassOf<UCameraShakeBase> mediumHoldStageBuildUp;

	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Camera Shake", meta = (ToolTip = "When the player is moving then the camera will shake under these settings"))
	TSubclassOf<UCameraShakeBase> longHoldStageBuildUp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gladiator Camera Post Process")
	UMaterialInterface* DamagePostProcessMat;

	
	
	
public:
	// Sets default values for this character's properties
	AGladiatorPlayerChar();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* playerInput) override;

	void Tick(float DeltaTime) override;
	
private:
	void InitAbilitySystemComp();
	void InitHUD() const;

	UPROPERTY()
	FVector2D mouseInput;
	float CameraOverTime;
	float PlayerOverTime;
	float TargetOverTime;
	
	
	float smoothYawInput = 0.0f;
	float smoothPitchInput = 0.0f;
	bool bIsLerping = false;
	bool bIsPlayerLerping = false;
	
	FVector cameraDirection;

protected:
	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Gladiator Input")// <-- gets set in the class defaults
	UInputAction* LookAction;
	
	void CameraInputCallback(const FInputActionInstance& instance);
	
	//sync the LerpInput() & LerpPlayerRotation() in one function
	void LerpCameraSystem(const FVector2D values);

	//lerps the camera based off the new input from the player
	void LerpInput(const FVector2D values, float time);

	void LerpToTarget(const AActor* target, float time);
	
	//slerps the player character to where the camera is looking 
	void LerpPlayerRotation(float time);

	//returns the calculated drag value based on the drag setting
	static float DetermineDragCalculation(EDragSettings DragType, const float alpha);

	
	
	UPROPERTY()
	AEnemyBase* CurrentLockedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gladiator Input")
	bool bPlayerIsSuckingToTarget = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsLocking = false;
	bool bCanBeLocked = false;

	UPROPERTY(EditAnywhere, Category = "Gladiator Lock On")
	EDragSettings LockOnDragSettings;

	UPROPERTY(EditAnywhere, Category = "Gladiator Lock On")
	UMaterialInterface* LockOnDecal;

	UDecalComponent* currentDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maestro")
	TSubclassOf<AMaestroBase> maestroClass;

	AMaestroBase* Maestro;
	
	UPROPERTY(EditAnywhere, Category = "Gladiator Lock On")
	FVector LockOnDecalSize;
	
	UPROPERTY(EditAnywhere, Category = "Gladiator Lock On")
	float LerpTimeToTarget = 1.0f;

	UPROPERTY(editAnywhere, Category = "Gladiator Lock On", meta = (ToolTip = "Determines how much the player can look around before the lock on system automatically deactivates",
		ClampMin = 0, ClampMax = 180, UIMin = "0", UIMax = "180"))
	float LookZoneBeforeDeactivate;

	UPROPERTY()
	int iterator = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Gladiator Lock On")
	TArray<FHitResult> CurrentHitResults;

	//grabs an array of all enemies that are hit and returns them
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	TArray<FHitResult> GetEnemiesInView();	

	//sets a valid locked target to CurrentLockedTarget and spawns in a decal as a marker to mark the target
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	void SetLockedTarget(AEnemyBase* enemies);

	//cycles to the next target in the array, once at the end it loops back to the beginning
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	AEnemyBase* CycleToNextTarget(TArray<FHitResult> enemies);

	//determines if the player is in view of the locked target which is dependent on the LookZoneBeforeDeactivate value
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	bool IsInViewOfTarget(AEnemyBase* Target);

	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	AEnemyBase* ClearLockOn();

	//when we deactivate the lock on we need to clear the currenttarget & destroy the decal
	UPROPERTY(BlueprintReadWrite, Category = "Gladiator Suck To Target")
	FVector2D InputActionValue; //store the Forward/Backward input action float value

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gladiator Suck To Target", meta=(ClampMin=0.1, ClampMax=2, ToolTip="When the player is moving forward and attacks, how much do we want to scale the range of the collision trace for Suck to Target"))
	float ForwardSTTMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gladiator Suck To Target", meta=(ClampMin=0.1, ClampMax=2, ToolTip="When the player is moving backward and attacks, how much do we want to scale the range of the collision trace for Suck to Target"))
	float BackwardSTTMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gladiator Suck To Target", meta=(ToolTip="When player attempts to attack we dont always want to STT if the player is in range to hit the enemy"))
	float STTDeadzone = 60.f;
	
	UFUNCTION(BlueprintCallable, Category = "Gladiator Suck To Target", meta=(ToolTip="Shoot a collision trace and return the first enemy hit"))
	FHitResult DetectEnemyToSuckTo(float Radius, EDrawDebugTrace::Type Debug, float debugTraceTime);
	
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	void EnemyHighlight(AEnemyBase* Enemy);
	
	UFUNCTION(BlueprintCallable, Category = "Gladiator Lock On")
	void EnemyDehighlight(AEnemyBase* Enemy);

	//to predict if the closest enemy (the assumed target) will die next
	UFUNCTION(BlueprintCallable, Category = "Gladiator Kill Camera")
	void PredictEnemyDeath(float searchRadius, EDrawDebugTrace::Type Debug, float debugTraceTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gladiator Kill Camera")
	TSubclassOf<AGladiatorCameraBase> FinalKillCamera;
	AActor* SpawnedKillCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UGladiatorCameraPositionComponent* KillCameraPosition;
	
	UFUNCTION(BlueprintCallable, Category = "Gladiator Kill Camera")
	void TransistionCameraTargetView(TSubclassOf<AGladiatorCameraBase> Target);

	void TransitionBackToMainCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputBuffer> inputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAcceptInputQueue = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Buffer")
	float tapAttackTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Buffer")
	float longHoldAttackTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Buffer", meta=(ToolTip="how long between each attack will be allowed before any prev attacks stored in memory get wiped"))
	float timeTillAttackMemoryWiped;
	
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void TryActivateAttackType();

	//when we want to add to the buffer 
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void AddToBuffer(FInputBuffer bufferToInsert);

	UFUNCTION(BlueprintNativeEvent, Category = "Input Buffer")
	void ActivatePreAttackAdjustment();

	//turns off bcanAcceptInputQueue
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void DisableInputQueuing();

	//turns on bCanAcceptInputQueue
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void EngageInputQueuing();

	UPROPERTY(EditDefaultsOnly, Category = "Hit stun")
	TSubclassOf<UGameplayAbility> IFramesAbiltiy;

	UPROPERTY(EditDefaultsOnly, Category = "Hit stun")
	int amountOfTimesHitTolerance;

	UPROPERTY(EditDefaultsOnly, Category = "Hit stun", meta=(ToolTip="How long till we reset how many times the player was hit consecutively from enemies"))
	float timeTillHitBufferExpires;

	UPROPERTY()
	EAttackHoldStage previousStage;
	
	//returns a stage that the attack is getting held at 
	UFUNCTION(BlueprintCallable, Category = "Attack Buildup")
	EAttackHoldStage DetermineCurrentHoldStage(float timeHeld);

	UFUNCTION(BlueprintCallable)
	void ActivateHoldStage(EAttackHoldStage stage);

	UFUNCTION(BlueprintCallable)
	void CompleteHoldStage();

	//how fast the wind up animation plays for to match the stages 
	UPROPERTY(BlueprintReadOnly)
	float windUpTimeSpeed;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maestro")
	TSubclassOf<AMaestroBase> maestroClass;

	AMaestroBase* Maestro;*/

	UFUNCTION(BlueprintCallable)
	void AdjustCameraZoom(float zoomValue, float time);

	UFUNCTION(BlueprintCallable)
	void ResetToDefaultZoom(float time);
	
	void ApplyZoom(FVector startingLoco,FVector zoomValue, float time);
private:
	APlayerCameraManager* CameraManager;
	UCameraComponent* CameraComponent;
	float gCurrentPlayerSpeed;

	UPROPERTY()
	UEnemySubsystem* EnemySubsystem;
	
	UPROPERTY()
	UCameraShakeBase* currentActiveCameraShake;

	UPROPERTY()
	UCameraShakeBase* currentActiveAttackHoldCameraShake;
	
	void UpdateMovementCameraShake(float speed);

	void UpdateCameraShake(TSubclassOf<UCameraShakeBase> shakeToActivate, UCameraShakeBase*& shakeValue);

	void ClearCameraShake(UCameraShakeBase*& shakeValue);
	
	FTimerHandle TimerHandle;
	float timerAlpha = 0.f;
	
	bool isRunning = false;

	UMaterialInstanceDynamic* PPDamagedMat;

	FTimerHandle combatTimerHandle;
	TArray<EAttackType> prevExecutedAttacks;

	void AddAttackToMemory(EAttackType type);
	
	void CreateAndApplyDynamicMaterialToCamera();
	
	void ApplyEffect(float timeToLerp);
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void InitInputBuffer();

	void ClearAttacksMemory();

	void SelectAttackToUse(FInputBuffer selectedBuffer);

	void TryActivateIframess();

	void ClearHitBuffer();

	USpringArmComponent* playerSpringArmComponent;

	FTimerHandle HitBufferHandle;
	int HitBuffer;

	FTimerHandle CameraZoomTimerHandle;
	FVector CameraDefaultPos;
	float cameraChangeAlpha = 0.f;
	
	
	
};

