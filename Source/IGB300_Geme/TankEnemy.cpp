// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEnemy.h"

void ATankEnemy::Move() {

}

void ATankEnemy::BeginPlay() {
	AEnemyBase::BeginPlay();
}

void ATankEnemy::Tick(float DeltaTime) {
	targetMovePos = enemyManager->ReturnPlayerPos(9);
	SetActorLocation(FMath::Lerp(GetActorLocation(), targetMovePos, DeltaTime * speed));
}