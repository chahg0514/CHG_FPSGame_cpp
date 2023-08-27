// Fill out your copyright notice in the Description page of Project Settings.


#include "CAIController.h"

#include "BlackboardKeys.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CTargetPoint_Real.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BTComp"));


	
	ConstructorHelpers::FObjectFinder<UBehaviorTree> LaserAsset(TEXT("BehaviorTree'/Game/Blueprint/Enemy/BT_Enemy.BT_Enemy'"));
	if (LaserAsset.Succeeded())
	{
		LaserTree = LaserAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UBehaviorTree> ProjAsset(TEXT("BehaviorTree'/Game/Blueprint/Enemy/BT_Enemy_Projectile.BT_Enemy_Projectile'"));
	if (ProjAsset.Succeeded())
	{
		ProjTree = ProjAsset.Object;
	}
	CurrentPatrolPoint = 0;
}
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACEnemy* Enemy = Cast<ACEnemy>(InPawn);
	if (Enemy->AttackType == EAttackType::VE_Laser)
	{
		if (IsValid(LaserTree)) BTree = LaserTree;
	}
	if (Enemy->AttackType == EAttackType::VE_Projectile)
	{
		if (IsValid(ProjTree)) BTree = ProjTree;
	}
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree); 
	
	//�����̺��Ʈ�� ����
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	//������ ������Ʈ�� �����̺��Ʈ���� �ִ� �����带 �־���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTargetPoint_Real::StaticClass(), PatrolPoints);
	//���忡 �ִ� Ÿ������Ʈ�� ��� ã��
	Blackboard->SetValueAsObject(BBKeys::PatrolPointKey, PatrolPoints[0]);
	//��Ʈ�� Ű�� TargetPoint ������Ʈ �ϳ��� �־���
}