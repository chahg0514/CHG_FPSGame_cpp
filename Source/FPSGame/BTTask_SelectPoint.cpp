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
		return EBTNodeResult::Failed; //���й�ȯ
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

	ACTargetPoint_Real* CurrentPoint = Cast<ACTargetPoint_Real>(BlackboardComp->GetValueAsObject(BBKeys::PatrolPointKey));
	//�����忡 ����� Ÿ������Ʈ�� ������
	TArray<AActor*> AvailablePatrolPoints = AIController->GetPatrolPoints(); //Ÿ������Ʈ �迭�� ������
	ACTargetPoint_Real* NextPatrolPoint = nullptr; //���� ���� ����
	if (AIController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1) //���� ���� ������ ������ ������ �ƴ϶��
	{
		NextPatrolPoint = Cast<ACTargetPoint_Real>(AvailablePatrolPoints[++AIController->CurrentPatrolPoint]); //���� ���������� �־���

	}
	else //������ �����̶��
	{
		NextPatrolPoint = Cast<ACTargetPoint_Real>(AvailablePatrolPoints[0]);
		AIController->CurrentPatrolPoint = 0;
	}
	BlackboardComp->SetValueAsObject(BBKeys::PatrolPointKey, NextPatrolPoint);
	//NextPatrolPoint = 

	return EBTNodeResult::Succeeded; //���� ��ȯ
}