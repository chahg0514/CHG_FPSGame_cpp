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
#include "Kismet/GameplayStatics.h" //�̰� ������ ����
#include "particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "CBullet.h"
#include "Materials/MaterialInstanceConstant.h"

// Sets default values
ACRifle::ACRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //ƽ�Լ� ������� �ʰڴٰ� �߾��µ� �ٽ� true�� �ٲ�
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
	//��Į
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
	//������ �޽��� Ȧ���� ���Ͽ� ��ӽ�Ŵ(���δٴ°��ε�?)
	
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
		Rifle->GetLocationAndDirection(Start, End, Direction); //�� �̰� ���� �����̶� �ڲ� �ʷϻ����� �Ⱥ��ϴ°ſ�����
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); //������ ����
		Params.AddIgnoredActor(OwnerCharacter); //������ ����
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params))
		{

			ACharacter* Other = Cast<ACharacter>(HitResult.GetActor());
			if (Other == nullptr) //�̰� ������ �𸮾� ƨ���
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

//���丮 ����
ACRifle* ACRifle::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters Params;
	Params.Owner = InOwner;

	return InWorld->SpawnActor<ACRifle>(Params);//������ ����
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
	//!isEquipping�� ������ false�ΰ� �ƴѰ� isEqipped�� true�̸�.. �׸��� ������ Equipped�϶��� isAiming �ٲٴµ� ���͵� �ʿ�����ʳ�
	if (isAiming && !isFiring)
	{
		Firing();

	}
}

void ACRifle::Firing()
{
	ACPlayer* MyPlayer = Cast<ACPlayer>(OwnerCharacter);

	if (MyPlayer == nullptr) return;
	MyPlayer->PlayCameraShake(); //ī�޶� ����

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
		FRotator Rot = HitResult.ImpactNormal.Rotation(); //���̿� �浹�� ��ü�� ��ֺ����� ȸ���� (�ݻ�Ǿ���� ����)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, Rot, FVector(2));
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		//Muzzle ���Ͽ� ��ƼŬ ����
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), HitResult.Location, Rot, 10.f);
	}

	



	if (isTarget && IsValid(Target))
	{
		UGameplayStatics::ApplyDamage(Target, 10.f, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());
		FVector Dir = Target->GetActorLocation() - OwnerCharacter->GetActorLocation(); //Ÿ�������� ���� ž�ٿ�
		Dir.Normalize();
		ACharacter* TargetCharacter = Cast<ACharacter>(Target);

		//if(TargetCharacter->GetMesh()->PhysicsSimul)
		if (IsValid(TargetCharacter))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fired"));
			//TargetCharacter->GetMesh()->AddImpulseAtLocation(Dir * TargetCharacter->GetMesh()->GetMass() * 100, OwnerCharacter->GetActorLocation());
			// Simulate Physics�� üũ�ϸ� ��������(���� � ���� ���� �����ļ� �ȹ�����=>�̰� Collision Enabled�� �ȵǾ��־ �׷���) �̰� �ּ�ó����
			//AddImpulseAtLocation : �޽�������Ʈ�� �浹ȿ���� ��
		}
	}
}

void ACRifle::EndFire()
{
}


