// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectileBullet.h"

#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"


#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyProjectileBullet::AEnemyProjectileBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Blueprint/Player/MAT_Bullet_Inst.MAT_Bullet_Inst'"));
	if (MaterialAsset.Succeeded())
	{
		//MatIns = MaterialAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_40mm_G.SM_Shell_40mm_G'"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
	Mesh->SetMaterial(0, MatIns);
	Mesh->SetRelativeScale3D(FVector(2));
	Mesh->SetupAttachment(Root);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectileBullet::OnOverlapBegin); //오류 Add 를 All이라 적음
	CollisionComp->SetupAttachment(Mesh);

	Projectile->InitialSpeed = 2000.f;
	Projectile->MaxSpeed = 2000.f;
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->SetUpdatedComponent(Root);
	InitialLifeSpan = 10.f; //10초후 제거 
}

// Called when the game starts or when spawned
void AEnemyProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &AEnemyProjectileBullet::OnHit);
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectileBullet::OnOverlapBegin);
	
}

// Called every frame
void AEnemyProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemyProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnHit"));
	ACharacter* Other = Cast<ACharacter>(OtherActor);
	if (Other == nullptr) //이거 없으면 언리얼 튕기네
	{
		return;
	}
	
	if (Other->ActorHasTag(FName(TEXT("Player"))))
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PlayerOnHit"));
		//UGameplayStatics::ApplyDamage(Target, 10.f, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());
		Destroy();
		return;
	}
	
}
void AEnemyProjectileBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PlayerOverlapBegin"));
		UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
	}
}
void AEnemyProjectileBullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PlayerAttackSuccess"));
}
//void AEnemyProjectileBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation());
//	//특정 위치에 파티클을 생성
//	Destroy(); //현재 액터 제거
//}


