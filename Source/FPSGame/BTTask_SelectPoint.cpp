// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectPoint.h"

#include "BlackboardKeys.h"
#include "CAIController.h"
#include "CTargetPoint_Real.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_SelectPoint.h"

UBTTask_SelectPoint::UBTTask_SelectPoint()
{
	NodeName = TEXT("SelectPoint");
}
EBTNodeResult::Type UBTTask_SelectPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return EBTNodeResult::Failed; //실패반환
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

	ACTargetPoint_Real* CurrentPoint = Cast<ACTargetPoint_Real>(BlackboardComp->GetValueAsObject(BBKeys::PatrolPointKey));
	//블랙보드에 저장된 타겟포인트를 가져옴
	TArray<AActor*> AvailablePatrolPoints = AIController->GetPatrolPoints(); //타겟포인트 배열을 가져옴
	ACTargetPoint_Real* NextPatrolPoint = nullptr; //다음 정찰 지점
	if (AIController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1) //현재 정찰 지점이 마지막 지점이 아니라면
	{
		NextPatrolPoint = Cast<ACTargetPoint_Real>(AvailablePatrolPoints[++AIController->CurrentPatrolPoint]); //다음 정찰지점을 넣어줌

	}
	else //마지막 지점이라면
	{
		NextPatrolPoint = Cast<ACTargetPoint_Real>(AvailablePatrolPoints[0]);
		AIController->CurrentPatrolPoint = 0;
	}
	BlackboardComp->SetValueAsObject(BBKeys::PatrolPointKey, NextPatrolPoint);
	//NextPatrolPoint = 

	return EBTNodeResult::Succeeded; //성공 반환
}