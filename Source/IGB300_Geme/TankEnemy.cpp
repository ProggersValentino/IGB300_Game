// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEnemy.h"
#include "EnemyManager.h"
#include "IGB300_Geme/EnemyType.h"

void ATankEnemy::Move_Implementation() {
	AEnemyBase::Move_Implementation();
}

void ATankEnemy::Attack_Implementation() {

}


void ATankEnemy::BeginPlay() {
	AEnemyBase::BeginPlay();
}

void ATankEnemy::Tick(float DeltaTime) {
	AEnemyBase::Tick(DeltaTime);
}

EEnemyType ATankEnemy::IsOfType(){
	return EEnemyType::ET_Tank;
}
