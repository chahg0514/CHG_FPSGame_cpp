// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_EnemyDamage.h"
#include "CEnemy.h"

FString UCAnimNotify_EnemyDamage::GetNotifyName_Implementation() const
{
	return TEXT("EnemyDamage");
}

void UCAnimNotify_EnemyDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACEnemy* Enemy = Cast<ACEnemy>(MeshComp->GetOwner());

	if (Enemy == nullptr) return;
	Enemy->EnemyFire();
}
