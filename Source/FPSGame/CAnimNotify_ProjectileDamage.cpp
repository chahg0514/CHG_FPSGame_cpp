// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_ProjectileDamage.h"
#include "CEnemy.h"

void UCAnimNotify_ProjectileDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{


	Super::Notify(MeshComp, Animation);
	ACEnemy* Enemy = Cast<ACEnemy>(MeshComp->GetOwner());

	if (Enemy == nullptr) return;
	Enemy->EnemyProjectileAttack();


}
