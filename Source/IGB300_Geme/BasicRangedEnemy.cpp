// Fill out your copyright notice in the Description page of Project Settings.
#include "BasicRangedEnemy.h"
#include "EnemyType.h"
#include "EnemyManager.h"
#include "IGB300_Geme/EnemyType.h"

void ABasicRangedEnemy::Move_Implementation() {
	AEnemyBase::Move_Implementation();
}

void ABasicRangedEnemy::Attack_Implementation() {

}

void ABasicRangedEnemy::Tick(float DeltaTime){
  AEnemyBase::Tick(DeltaTime);
}
void ABasicRangedEnemy::BeginPlay() {
  AEnemyBase::BeginPlay();

  /*  Old logic sets the enemy to run at player location, will always go 0,0 if enemy exists on startup
      Will be okay logic when spawning a enemy during level
      
  if (enemyManager)
	  targetMovePos = enemyManager->ReturnPlayerPos(0);
	*/
}

EEnemyType ABasicRangedEnemy::IsOfType(){
  return EEnemyType::ET_Ranged;
}
