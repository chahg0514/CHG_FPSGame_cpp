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
	
	//비헤이비어트리 시작
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	//블랙보드 컴포넌트에 비헤이비어트리에 있는 블랙보드를 넣어줌
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTargetPoint_Real::StaticClass(), PatrolPoints);
	//월드에 있는 타겟포인트를 모두 찾음
	Blackboard->SetValueAsObject(BBKeys::PatrolPointKey, PatrolPoints[0]);
	//패트롤 키에 TargetPoint 오브젝트 하나를 넣어줌
}