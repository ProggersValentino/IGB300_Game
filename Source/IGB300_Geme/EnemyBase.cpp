// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyManager.h"
#include "EnemyType.h"
#include "Components/CapsuleComponent.h"
#include "GAS/GladiatorAbilitySystemComponent.h"
#include "GAS/GladiatorAttributeSet.h"
#include "IGB300_Geme/EnemyType.h"
#include "UObject/ReferenceChainSearch.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	targetMovePos = FVector(0.0f, 0.0f, 0.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UGladiatorAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGladiatorAttributeSet>("AttributeSet");
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	//ability setup releated
	AbilitySystemComponent->InitAbilityActorInfo(this, this); //assigning the enemy its ability actor info for server and local
	GiveDefaultAbilities();
	InitDefaultAttributes();

	//binding to the health attribute so when it changes the function gets called
	HealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyBase::HealthChanged);
	
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

bool AEnemyBase::CanFinish_Implementation()
{
	return health < 10; //if health is critical then return true - 10 is just temp number
}

void AEnemyBase::GetExecuted_Implementation(UAnimMontage* animation)
{
	USkeletalMeshComponent* mesh = FindComponentByClass<USkeletalMeshComponent>(); 
	UAnimInstance* instance = mesh->GetAnimInstance();

	canMove = false;
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *instance->GetName());

	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (!instance || !animation) return; //are our references of instance and animation not null

	instance->OnPlayMontageNotifyEnd.AddDynamic(this, &AEnemyBase::OnNotifyEnd);
	
	//if not playing this montage -> play it
	if (!instance->Montage_IsPlaying(animation))
	{
		instance->Montage_Play(animation); //play montage
	}
}

void AEnemyBase::Move_Implementation(){

	if (!canMove) return; //if the enemy cant move then dont execute below
	
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

void AEnemyBase::Damage_Implementation(float amount){
	health -= amount;
	if (health < 0){
		//Die_Implementation();
	}
}

void AEnemyBase::Block_Implementation()
{
	IIEnemy::Block_Implementation();
}

EEnemyType AEnemyBase::IsOfType() {
	return EEnemyType::ET_Basic;
}

void AEnemyBase::HealthChanged(const FOnAttributeChangeData& Data)
{

	//kill enemy
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		enemyManager->DeregisterEnemy(this);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		Die();
	}
}

/// For when we have Montages and we want to execute code when a montage Notify State finishes 
/// @param NotifyName the name of the Notify State created in the Montage
/// @param Payload 
void AEnemyBase::OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	UAnimInstance* instance = FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	//if the notify state that ended is call "Executed" do this
	if (NotifyName == "Executed")
	{
		Damage_Implementation(200); //damages enemy
		instance->OnPlayMontageNotifyEnd.RemoveAll(this); //cleanup once done
	}
}

