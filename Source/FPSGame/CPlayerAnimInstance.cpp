// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerAnimInstance.h"
#include "CPlayer.h"
#include "CRifle.h"
#include "IRifle.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ACPlayer>(TryGetPawnOwner()); //오너를 갖고와서 캐스트해서 캐릭터변수 갖고옴 이거 기억해두자
}

void UCPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(OwnerCharacter))
	{
		Speed = OwnerCharacter->GetVelocity().Size2D(); //velocity를 사이즈하면 속도가 나오나봐 이거도 기억
		

		//낙하여부
		isAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
		Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
		//GetBaseAimRotation(): Pawn의 조준점
		//CalculateDiraction: 두 방향의 사이각을 반환
		IIRifle* rifle = Cast<IIRifle>(OwnerCharacter);
		if (rifle != nullptr)
		{
			//무기 장착여부를 갖고옴
			isEquipped = rifle->GetRifle()->GetIsEquipped();
			//조준여부
			isAiming = rifle->GetRifle()->GetIsAiming();

		}
	}
}
