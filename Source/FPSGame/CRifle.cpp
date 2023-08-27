// Fill out your copyright notice in the Description page of Project Settings.


#include "CRifle.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "IRifle.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayer.h"
#include "Kismet/GameplayStatics.h" //이거 뭔지좀 보자
#include "particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "CBullet.h"
#include "Materials/MaterialInstanceConstant.h"

// Sets default values
ACRifle::ACRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //틱함수 사용하지 않겠다고 했었는데 다시 true로 바꿈
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetSkeletalMesh(MeshAsset.Object);
	}
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GrabMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Player/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
	if (GrabMtgAsset.Succeeded())
	{
		GrabMontage = GrabMtgAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnGrabMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Player/UnEquip_Rifle_Standing_Montage.UnEquip_Rifle_Standing_Montage'"));
	if (UnGrabMtgAsset.Succeeded())
	{
		UnGrabMontage = UnGrabMtgAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Player/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage'"));
	if (FireMtgAsset.Succeeded())
	{
		FireMontage = FireMtgAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleAsset(TEXT("ParticleSystem'/Game/Blueprint/Player/P_FireBall_Powerup.P_FireBall_Powerup'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactAsset(TEXT("ParticleSystem'/Game/Blueprint/Player/P_Impact_Shield_Ice.P_Impact_Shield_Ice'"));
	if (MuzzleAsset.Succeeded())
	{
		MuzzleParticle = MuzzleAsset.Object;
	}
	if (ImpactAsset.Succeeded())
	{
		ImpactParticle = ImpactAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundAsset(TEXT("SoundCue'/Game/Blueprint/Player/Pistol_Cue.Pistol_Cue'"));
	if (FireSoundAsset.Succeeded())
	{
		FireSoundCue = FireSoundAsset.Object;
	}
	//데칼
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Blueprint/Player/MAT_BulletMark_Inst.MAT_BulletMark_Inst'"));
	if (MaterialAsset.Succeeded())
	{
		DecalMaterial = MaterialAsset.Object;
	}

}

// Called when the game starts or when spawned
void ACRifle::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocket);
	//주인의 메쉬에 홀스터 소켓에 상속시킴(붙인다는거인듯?)
	
}

void ACRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!isAiming)
	{
		return;
	}
	//Rifle = dynamic_cast<IIRifle>(OwnerCharacter);
	IIRifle* Rifle = Cast<IIRifle>(OwnerCharacter);
	if (Rifle == nullptr)
	{
		return;
	}
	else
	{
		FVector Start, End, Direction;
		Rifle->GetLocationAndDirection(Start, End, Direction); //아 이게 완전 랜덤이라 자꾸 초록색으로 안변하는거였구나
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); //무시할 액터
		Params.AddIgnoredActor(OwnerCharacter); //무시할 액터
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params))
		{

			ACharacter* Other = Cast<ACharacter>(HitResult.GetActor());
			if (Other == nullptr) //이거 없으면 언리얼 튕기네
			{
				return;
			}
			if (Other->ActorHasTag(FName(TEXT("Enemy"))))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("EnemyDetected"));
				Rifle->OnFocus();
				isTarget = true;
				Target = Other;
				//UGameplayStatics::ApplyDamage(Target, 10.f, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());
				return;
			}

			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("EnemyNotDetected"));
			isTarget = false;
			Rifle->OffFocus();
			Target = nullptr;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("EnemyNotDetected"));
	isTarget = false;
	Rifle->OffFocus();
	Target = nullptr;


}

//팩토리 패턴
ACRifle* ACRifle::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters Params;
	Params.Owner = InOwner;

	return InWorld->SpawnActor<ACRifle>(Params);//라이플 생성
}

void ACRifle::Equip()
{
	if (!isEquipped && !isEquipping)
	{
		isEquipping = true;
		OwnerCharacter->PlayAnimMontage(GrabMontage);
	}
}

void ACRifle::BeginEquip()
{
	isEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HandSocket);
}

void ACRifle::EndEquip()
{
	isEquipping = false;
}

void ACRifle::UnEquip()
{
	if (isEquipped && !isEquipping)
	{
		isEquipping = true;
		OwnerCharacter->PlayAnimMontage(UnGrabMontage);
	}
}

void ACRifle::BeginUnEquip()
{
	isEquipped = false;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocket);
}

void ACRifle::EndUnEquip()
{
	isEquipping = false;
}

void ACRifle::BeginFire()
{
	//!isEquipping은 언제나 false인거 아닌가 isEqipped가 true이면.. 그리고 어차피 Equipped일때만 isAiming 바꾸는데 저것도 필요없지않나
	if (isAiming && !isFiring)
	{
		Firing();

	}
}

void ACRifle::Firing()
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(OwnerCharacter);

	if (MyPlayer == nullptr) return;
	MyPlayer->PlayCameraShake(); //카메라 흔들기

	OwnerCharacter->PlayAnimMontage(FireMontage);

	FVector MuzzleLocation = Mesh->GetSocketLocation(FName(TEXT("Muzzle")));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, MuzzleLocation);

	

	FVector Start, End, Direction;

	IIRifle* Rifle = Cast<IIRifle>(OwnerCharacter);
	Rifle->GetLocationAndDirection(Start, End, Direction);

	GetWorld()->SpawnActor<ACBullet>(ACBullet::StaticClass(), MuzzleLocation,Direction.Rotation());
	//GetWorld()->SpawnActor<ACBullet>(ACBullet::StaticClass(), Direction.Rotation());

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerCharacter);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		FRotator Rot = HitResult.ImpactNormal.Rotation(); //레이와 충돌한 물체의 노멀벡터의 회전값 (반사되어나오는 각도)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, Rot, FVector(2));
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		//Muzzle 소켓에 파티클 생성
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), HitResult.Location, Rot, 10.f);
	}

	



	if (isTarget && IsValid(Target))
	{
		UGameplayStatics::ApplyDamage(Target, 10.f, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());
		FVector Dir = Target->GetActorLocation() - OwnerCharacter->GetActorLocation(); //타겟으로의 방향 탑다운
		Dir.Normalize();
		ACharacter* TargetCharacter = Cast<ACharacter>(Target);

		//if(TargetCharacter->GetMesh()->PhysicsSimul)
		if (IsValid(TargetCharacter))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fired"));
			//TargetCharacter->GetMesh()->AddImpulseAtLocation(Dir * TargetCharacter->GetMesh()->GetMass() * 100, OwnerCharacter->GetActorLocation());
			// Simulate Physics를 체크하면 무너져서(나는 어떤 오류 아직 못고쳐서 안무너짐=>이거 Collision Enabled가 안되어있어서 그랬음) 이거 주석처리함
			//AddImpulseAtLocation : 메쉬컴포넌트에 충돌효과를 줌
		}
	}
}

void ACRifle::EndFire()
{
}


