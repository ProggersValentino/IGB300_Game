// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMeleeEnemy.h"

void ABasicMeleeEnemy::Move() {

}

void ABasicMeleeEnemy::BeginPlay() {
	AEnemyBase::BeginPlay();
}

void ABasicMeleeEnemy::Tick(float DeltaTime) {
	AEnemyBase::Tick(DeltaTime);
	if (enemyManager)
		targetMovePos = enemyManager->ReturnPlayerPos(0);
	SetActorLocation(FMath::Lerp(GetActorLocation(), targetMovePos, DeltaTime * speed));
}

