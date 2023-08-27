// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotifyState_Equip.h"
#include "IRifle.h"
#include "CRifle.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return TEXT("Equip");
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TitalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TitalDuration);
	if (MeshComp != nullptr)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (rifle != nullptr)
		{
			rifle->GetRifle()->BeginEquip();
		}
	}
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp != nullptr)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (rifle != nullptr)
		{
			rifle->GetRifle()->EndEquip();
		}
	}
}
