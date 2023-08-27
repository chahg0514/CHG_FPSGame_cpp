// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotifyState_UnEquip.h"
#include "IRifle.h"
#include "CRifle.h"

FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return TEXT("UnEquip");
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TitalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TitalDuration);
	if (MeshComp != nullptr)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (rifle != nullptr)
		{
			rifle->GetRifle()->BeginUnEquip();
		}
	}
}

void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp != nullptr)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (rifle != nullptr)
		{
			rifle->GetRifle()->EndUnEquip();
		}
	}
}
