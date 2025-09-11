// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/GladiatorCameraPositionComponent.h"

// Sets default values for this component's properties
UGladiatorCameraPositionComponent::UGladiatorCameraPositionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	/*CameraFacingDirection->SetupAttachment(this);*/
}


// Called when the game starts
void UGladiatorCameraPositionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGladiatorCameraPositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGladiatorCameraPositionComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraFacingDirection)
	{
		CameraFacingDirection = NewObject<UArrowComponent>(this, TEXT("CameraFacingDirection"));
		CameraFacingDirection->RegisterComponent();
		CameraFacingDirection->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

