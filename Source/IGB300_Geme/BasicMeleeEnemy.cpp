// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMeleeEnemy.h"
#include "EnemyManager.h"
#include "IGB300_Geme/EnemyType.h"

void ABasicMeleeEnemy::Move_Implementation() {
	AEnemyBase::Move_Implementation();
}

void ABasicMeleeEnemy::Attack_Implementation() {

}

void ABasicMeleeEnemy::BeginPlay() {
	AEnemyBase::BeginPlay();
}

void ABasicMeleeEnemy::Tick(float DeltaTime) {
	AEnemyBase::Tick(DeltaTime);
}

EEnemyType ABasicMeleeEnemy::IsOfType(){
	return EEnemyType::ET_Basic;
}
