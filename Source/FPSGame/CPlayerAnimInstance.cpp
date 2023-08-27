// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerAnimInstance.h"
#include "CPlayer.h"
#include "CRifle.h"
#include "IRifle.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ACPlayer>(TryGetPawnOwner()); //���ʸ� ����ͼ� ĳ��Ʈ�ؼ� ĳ���ͺ��� ����� �̰� ����ص���
}

void UCPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(OwnerCharacter))
	{
		Speed = OwnerCharacter->GetVelocity().Size2D(); //velocity�� �������ϸ� �ӵ��� �������� �̰ŵ� ���
		

		//���Ͽ���
		isAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
		Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
		//GetBaseAimRotation(): Pawn�� ������
		//CalculateDiraction: �� ������ ���̰��� ��ȯ
		IIRifle* rifle = Cast<IIRifle>(OwnerCharacter);
		if (rifle != nullptr)
		{
			//���� �������θ� �����
			isEquipped = rifle->GetRifle()->GetIsEquipped();
			//���ؿ���
			isAiming = rifle->GetRifle()->GetIsAiming();

		}
	}
}
