// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEnemy.h"

void ATankEnemy::Move_Implementation() {

}

void ATankEnemy::Attack_Implementation() {

}


void ATankEnemy::BeginPlay() {
	AEnemyBase::BeginPlay();
}

void ATankEnemy::Tick(float DeltaTime) {
	AEnemyBase::Tick(DeltaTime);
	if (enemyManager)
		targetMovePos = enemyManager->ReturnPlayerPos(9);
	SetActorLocation(FMath::Lerp(GetActorLocation(), targetMovePos, DeltaTime * speed));
}
