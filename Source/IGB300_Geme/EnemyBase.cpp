// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyManager.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	targetMovePos = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AActor* enemyManAct = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
	if (enemyManAct)
		enemyManager = Cast<AEnemyManager>(enemyManAct);
	UID = enemyManager->RegisterEnemy(this);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move_Implementation();
}

bool AEnemyBase::CanDoFinisher() {
	return false;
}

void AEnemyBase::Move_Implementation(){
	// Distance between target move position and position after moving max speed to desired location
	FVector n = targetMovePos - GetActorLocation();
	UKismetMathLibrary::Vector_Normalize(n);
	FVector bestPossiblePos = n * FVector(speed, speed, 0.0f) + GetActorLocation();
	float a = UKismetMathLibrary::Vector_Distance(bestPossiblePos, targetMovePos);

	// Distance between current location and target position
	float b = UKismetMathLibrary::Vector_Distance(targetMovePos, GetActorLocation());

	if (a > b) {
		SetActorLocation(targetMovePos);
	}
	else {
		SetActorLocation(bestPossiblePos);
	}
}
void AEnemyBase::Attack_Implementation(){
	
}
void AEnemyBase::Die_Implementation(){
	enemyManager->DeregisterEnemy(this);
	Destroy();
}
void AEnemyBase::Damage_Implementation(float amount){
	health -= amount;
	if (health < 0){
		Die_Implementation();
	}
}

