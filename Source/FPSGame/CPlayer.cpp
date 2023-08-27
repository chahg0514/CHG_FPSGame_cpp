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
	GetCharacterMovement()->bOrientRotationToMovement = true; //이거 false로 하면 회전안함
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	Tags.Add(FName(TEXT("Player")));
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0); //이게 약간 오른쪽으로 치우치게해주는 역할

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Player/ABP_CPlayer.ABP_CPlayer_C'")); //애님인스턴스를 갖고오네?
	if (AnimAsset.Succeeded())
	{
		//GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
		GetMesh()->SetAnimClass(AnimAsset.Class); //f12 눌러보면 둘이 걍 똑같음 어이없네
	}
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Player/WB_CrossHair.WB_CrossHair_C'"));
	if (WidgetAsset.Succeeded())
	{
		CrossHairClass = WidgetAsset.Class;
	}
	//미니맵
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
		MinimapCamera->TextureTarget = MinimapTexture.Object; //씬캡쳐카메라가 보고있는 모습을 캔버스랜터타겟에 이미지하게 해줌,
	}

}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	Rifle = ACRifle::Spawn(GetWorld(), this); //총생성
	CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass); //위젯 생성
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
	//달리기
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Pressed, this, &ACPlayer::OnRunning);
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Released, this, &ACPlayer::OffRunning);
	//점프
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACPlayer::StopJumping);

	//무기장착
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ACPlayer::OnRifle);

	//조준
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ACPlayer::OffAim);

	//공격
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACPlayer::OnFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ACPlayer::OffFire);


}

void ACPlayer::MoveForward(float Axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0); 
	//컨트롤러의 Yaw축 회전값을 가져옴
	FVector Direction = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D(); //높이는 필요없기때문에 2D, z가 0으로 되는거임
	//Yaw축 기준으로 Forward방향을 구한 뒤 정규화를 시켜줌
	//정규화: 벡터의 길이를 1로 만들어줌
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
	
	if (Rifle->GetIsEquipped()) //장착중이라면
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
		bUseControllerRotationYaw = true; //마우스 회전(컨트롤러 회전이 마우스 회전인가봐)에 맞춰서 캐릭터도 회전
		GetCharacterMovement()->bOrientRotationToMovement = false; //무조건 앞만 보도록, 가는방향에 따라 회전 금지
		isAiming = true;
		SpringArm->SocketOffset = FVector(0, 30, 10);
		Camera->FieldOfView = 40; //시야범위
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
		Camera->FieldOfView = 90; //시야범위
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
	//카메라 앞방향
	OutDirection = Camera->GetForwardVector();

	//카메라의 월드 트랜스폼
	FTransform Transform = Camera->GetComponentToWorld();

	//카메라의 월드 위치
	FVector CameraLocation = Transform.GetLocation();

	//카메라의 위치 + 앞방향(위치 기준 앞방향)
	OutStart = CameraLocation + OutDirection;

	//균일한 무작위 분포로 지정된 원뿔 내에서 길이가 1인 무작위 벡터
	FVector ConDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);

	ConDirection *= 3000.f; //벡터 길이 3000으로 늘려줌

	OutEnd = CameraLocation + ConDirection;

}

void ACPlayer::PlayCameraShake()
{
	GetController<APlayerController>()->PlayerCameraManager->PlayCameraShake(UCMatineeCameraShake::StaticClass()); //원래 PlayCameraShake 함수가 존재함
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

