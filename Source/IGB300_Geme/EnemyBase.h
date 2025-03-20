// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyManager.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UENUM(BluePrintType)
enum class EEnemyType : uint8
{
	ET_Basic	UMETA(DisplayName = "Basic"),
	ET_Ranged	UMETA(DisplayName = "Ranged"),
	ET_Tank		UMETA(DisplayName = "Tank")
};

UCLASS()
class IGB300_GEME_API AEnemyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

	int32 UID;

	UPROPERTY(EditAnywhere, Category = "Members")
	float health;

	UPROPERTY(EditAnywhere, Category = "Members")
	float speed;

	UPROPERTY(EditAnywhere, Category = "Members")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Members")
	float crowdXPWhenKilled;

	UPROPERTY(EditAnywhere, Category = "Members")
	float crowdXPWhenFinished;

	float lastTimeHitByplayer;
	float timeAlive;

	UPROPERTY(EditAnywhere, Category = "Members")
	FVector targetMovePos;

	AEnemyManager* enemyManager;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Move();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Die();
	virtual bool CanDoFinisher();


};
