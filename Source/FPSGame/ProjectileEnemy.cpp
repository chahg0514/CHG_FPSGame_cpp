// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEnemy.h"

#include "CRifle.h"
#include "CAIController.h"
#include "BlackboardKeys.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AProjectileEnemy::AProjectileEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Enemy/ABP_Enemy.ABP_Enemy_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAnimAsset(TEXT("AnimMontage'/Game/Blueprint/Enemy/Hit_React_1_Montage.Hit_React_1_Montage'"));
	if (HitAnimAsset.Succeeded())
	{
		HitMtg = HitAnimAsset.Object;
	}
	AIControllerClass = ACAIController::StaticClass(); //AI컨트롤러 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//월드에 있거나 스폰될 때 AI와 연결
	Tags.Add(FName(TEXT("Enemy")));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimAsset(TEXT("AnimMontage'/Game/Blueprint/Enemy/Fire_Shotgun_Ironsights_Montage.Fire_Shotgun_Ironsights_Montage'"));
	if (FireAnimAsset.Succeeded())
	{
		FireMontage = FireAnimAsset.Object;
	}
}

// Called when the game starts or when spawned
void AProjectileEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AProjectileEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AProjectileEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

