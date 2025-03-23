// Fill out your copyright notice in the Description page of Project Settings.
#include "BasicRangedEnemy.h"

void ABasicRangedEnemy::Move_Implementation() {
	SetActorLocation(FMath::Lerp(GetActorLocation(), targetMovePos, 0.1f * speed));
}

void ABasicRangedEnemy::Attack_Implementation() {

}

void ABasicRangedEnemy::Die_Implementation() {

}

void ABasicRangedEnemy::Damage_Implementation(float amount) {

}

void ABasicRangedEnemy::Tick(float DeltaTime){
  AEnemyBase::Tick(DeltaTime);
  Move_Implementation();
}
void ABasicRangedEnemy::BeginPlay() {
  AEnemyBase::BeginPlay();

  /*  Old logic sets the enemy to run at player location, will always go 0,0 if enemy exists on startup
      Will be okay logic when spawning a enemy during level
      
  if (enemyManager)
	  targetMovePos = enemyManager->ReturnPlayerPos(0);
	*/
}
