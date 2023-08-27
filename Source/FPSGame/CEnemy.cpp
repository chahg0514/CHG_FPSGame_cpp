// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"

#include "CRifle.h"
#include "CAIController.h"
#include "BlackboardKeys.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyProjectileBullet.h"

// Sets default values
ACEnemy::ACEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0,-90.f,0));
	//애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Enemy/ABP_Enemy.ABP_Enemy_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	//히트 몽타주
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
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ProjAnimAsset(TEXT("AnimMontage'/Game/Blueprint/Enemy/Projectile_Shotgun_Ironsights_Montage1.Projectile_Shotgun_Ironsights_Montage1'"));
	if (ProjAnimAsset.Succeeded())
	{
		ProjectileMontage = ProjAnimAsset.Object;
	}

}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	Rifle = ACRifle::Spawn(GetWorld(), this); //총생성
	Rifle->BeginEquip();
}

// Called every frame
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	currentHP -= Damage;
	PlayAnimMontage(HitMtg, 1.2f);
	if (currentHP <= 0)
	{
		Rifle->Destroy();
		Destroy();
	}
	return Damage;
}

void ACEnemy::PlayAttackMontage()
{
	PlayAnimMontage(FireMontage);
}

void ACEnemy::PlayProjectileAttackMontage()
{
	PlayAnimMontage(ProjectileMontage);
}


void ACEnemy::EnemyFire()
{
	ACAIController* EnemyController = Cast<ACAIController>(GetController());
	if (EnemyController == nullptr) return;
	AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetKey));
	if (Target == nullptr) return;
	FVector Start, End, Direction;

	Start = Rifle->GetMuzzleLocation(); //시작점은 총구위치
	Direction = (Target->GetActorLocation() - Start).GetSafeNormal();
	//GetSafeNormal():  정규화(길이 1로)
	End = Direction * 800 + Start;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Rifle);
	FHitResult HitResult;

	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.5f);
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params))
	{
		if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PlayerAttackSuccess"));
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.f, GetController(), this, UDamageType::StaticClass());
		}
	}

}

void ACEnemy::EnemyProjectileAttack()
{
	ACAIController* EnemyController = Cast<ACAIController>(GetController());
	if (EnemyController == nullptr) return;
	AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetKey));
	if (Target == nullptr) return;
	FVector Start, End, Direction;

	Direction = Target->GetActorLocation() - GetActorLocation();
	GetWorld()->SpawnActor<AEnemyProjectileBullet>(AEnemyProjectileBullet::StaticClass(), GetActorLocation(), Direction.Rotation());



	//Start = Rifle->GetMuzzleLocation(); //시작점은 총구위치
	//Direction = (Target->GetActorLocation() - Start).GetSafeNormal();
	////GetSafeNormal():  정규화(길이 1로)
	//End = Direction * 800 + Start;

	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);
	//Params.AddIgnoredActor(Rifle);
	//FHitResult HitResult;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.5f);
	//if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params))
	//{
	//	if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
	//	{
	//		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.f, GetController(), this, UDamageType::StaticClass());
	//	}
	//}
}

