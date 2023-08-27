// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_Detect.h"

#include "BlackboardKeys.h"
#include "CAIController.h"
#include "CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UMyBTService_Detect::UMyBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f; //ƽ�Լ� ���� �ֱ�
	RandomDeviation = 0.f;
}

void UMyBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return;
	FVector Center = ControllingPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	//GetWorld()->trace

	bool bResult = GetWorld()->OverlapMultiByChannel(OverlapResult, Center, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams); //���Ǿ� Ʈ���̽� ����?
	//DrawDebugSphere(GetWorld(), Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr);
		for (auto result : OverlapResult)
		{
			if (ACharacter* Other = Cast<ACharacter>(result.GetActor()))
			{
				//�÷��̾� ��Ʈ�ѷ��� �ִٸ�
				if (Other->GetController()->IsPlayerController()) //�̰� controller �θ��Լ����� �Ƹ� Possess?�� �÷��̾�� true�� ���ֳ��� �׷� ���� ���ɵ�
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, result.GetActor()); //Ÿ���� ����
					DrawDebugSphere(GetWorld(), Center, DetectRadius, 16, FColor::Green, false, 0.2f);

					DrawDebugPoint(GetWorld(), Other->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);//���� ǥ��

					DrawDebugLine(GetWorld(), Center, Other->GetActorLocation(), FColor::Cyan, false, 0.2f);
					return;
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr); //null������
	}

}
