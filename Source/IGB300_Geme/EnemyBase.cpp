// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AActor* enemyManAct = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
	if (enemyManAct)
		enemyManager = Cast<AEnemyManager>(enemyManAct);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::Attack() {

}

void AEnemyBase::Die() {

}

void AEnemyBase::Move() {

}

bool AEnemyBase::CanDoFinisher() {
	return false;
}

