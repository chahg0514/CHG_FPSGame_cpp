// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CRifle.h"
#include "CUserWidget_CrossHair.h"
#include "Kismet/KismetMathLibrary.h"
#include "CMatineeCameraShake.h"

#include "Engine/CanvasRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ACPlayer::ACPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; //�̰� false�� �ϸ� ȸ������
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	Tags.Add(FName(TEXT("Player")));
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0); //�̰� �ణ ���������� ġ��ġ�����ִ� ����

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Player/ABP_CPlayer.ABP_CPlayer_C'")); //�ִ��ν��Ͻ��� �������?
	if (AnimAsset.Succeeded())
	{
		//GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
		GetMesh()->SetAnimClass(AnimAsset.Class); //f12 �������� ���� �� �Ȱ��� ���̾���
	}
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Player/WB_CrossHair.WB_CrossHair_C'"));
	if (WidgetAsset.Succeeded())
	{
		CrossHairClass = WidgetAsset.Class;
	}
	//�̴ϸ�
	MinimapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapArm"));
	MinimapArm->bDoCollisionTest = false;
	MinimapArm->SetupAttachment(GetMesh());
	MinimapArm->SetRelativeLocation(FVector(0, 0, 90));
	MinimapArm->SetRelativeRotation(FRotator(-90, 90, 0));

	MinimapCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCamera"));
	MinimapCamera->SetupAttachment(MinimapArm);
	MinimapCamera->SetRelativeLocation(FVector(0));
	MinimapCamera->SetRelativeRotation(FRotator(0));
	MinimapArm->TargetArmLength = 700.f;

	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> MinimapTexture(TEXT("CanvasRenderTarget2D'/Game/Blueprint/Player/CRT_Minimap.CRT_Minimap'"));
	if (MinimapTexture.Succeeded())
	{
		MinimapCamera->TextureTarget = MinimapTexture.Object; //��ĸ��ī�޶� �����ִ� ����� ĵ��������Ÿ�ٿ� �̹����ϰ� ����,
	}

}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	Rifle = ACRifle::Spawn(GetWorld(), this); //�ѻ���
	CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass); //���� ����
	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Hidden);
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isAiming && SpringArm->TargetArmLength > 100.f) 
	{
		SpringArm->TargetArmLength -= 1;
	}
	else if (!isAiming && SpringArm->TargetArmLength < 200.f)
	{
		SpringArm->TargetArmLength += 1;
	}

}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACPlayer::LookUp);
	//�޸���
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Pressed, this, &ACPlayer::OnRunning);
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Released, this, &ACPlayer::OffRunning);
	//����
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACPlayer::StopJumping);

	//��������
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ACPlayer::OnRifle);

	//����
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ACPlayer::OffAim);

	//����
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACPlayer::OnFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ACPlayer::OffFire);


}

void ACPlayer::MoveForward(float Axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0); 
	//��Ʈ�ѷ��� Yaw�� ȸ������ ������
	FVector Direction = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D(); //���̴� �ʿ���⶧���� 2D, z�� 0���� �Ǵ°���
	//Yaw�� �������� Forward������ ���� �� ����ȭ�� ������
	//����ȭ: ������ ���̸� 1�� �������
	AddMovementInput(Direction, Axis);
}

void ACPlayer::MoveRight(float Axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();
	AddMovementInput(Direction, Axis);
}

void ACPlayer::Turn(float Axis)
{
	AddControllerYawInput(Axis);
}

void ACPlayer::LookUp(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ACPlayer::OnRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ACPlayer::OffRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void ACPlayer::OnRifle()
{
	
	if (Rifle->GetIsEquipped()) //�������̶��
	{
		Rifle->UnEquip();
		return;
	}
	Rifle->Equip();
}

void ACPlayer::OnAim()
{
	if (Rifle->GetIsEquipped() && !Rifle->GetIsEquipping())
	{
		bUseControllerRotationYaw = true; //���콺 ȸ��(��Ʈ�ѷ� ȸ���� ���콺 ȸ���ΰ���)�� ���缭 ĳ���͵� ȸ��
		GetCharacterMovement()->bOrientRotationToMovement = false; //������ �ո� ������, ���¹��⿡ ���� ȸ�� ����
		isAiming = true;
		SpringArm->SocketOffset = FVector(0, 30, 10);
		Camera->FieldOfView = 40; //�þ߹���
		Rifle->SetIsAiming(isAiming);
		CrossHair->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void ACPlayer::OffAim()
{
	if (Rifle->GetIsEquipped() && !Rifle->GetIsEquipping())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		isAiming = false;
		SpringArm->SocketOffset = FVector(0, 60, 0);
		Camera->FieldOfView = 90; //�þ߹���
		Rifle->SetIsAiming(isAiming);
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnFire()
{
	Rifle->BeginFire();
}

void ACPlayer::OffFire()
{
	Rifle->EndFire();
}

void ACPlayer::OnFocus()
{
	CrossHair->OnFocus();
}

void ACPlayer::OffFocus()
{
	CrossHair->OffFocus();
}

void ACPlayer::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	//ī�޶� �չ���
	OutDirection = Camera->GetForwardVector();

	//ī�޶��� ���� Ʈ������
	FTransform Transform = Camera->GetComponentToWorld();

	//ī�޶��� ���� ��ġ
	FVector CameraLocation = Transform.GetLocation();

	//ī�޶��� ��ġ + �չ���(��ġ ���� �չ���)
	OutStart = CameraLocation + OutDirection;

	//������ ������ ������ ������ ���� ������ ���̰� 1�� ������ ����
	FVector ConDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);

	ConDirection *= 3000.f; //���� ���� 3000���� �÷���

	OutEnd = CameraLocation + ConDirection;

}

void ACPlayer::PlayCameraShake()
{
	GetController<APlayerController>()->PlayerCameraManager->PlayCameraShake(UCMatineeCameraShake::StaticClass()); //���� PlayCameraShake �Լ��� ������
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	currentHP -= Damage;
	if (currentHP <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerDie"));
	}
	return Damage;
}

