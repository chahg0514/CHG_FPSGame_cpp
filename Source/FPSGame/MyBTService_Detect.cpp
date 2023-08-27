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
	Interval = 1.f; //틱함수 실행 주기
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
		ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams); //스피어 트레이스 생성?
	//DrawDebugSphere(GetWorld(), Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr);
		for (auto result : OverlapResult)
		{
			if (ACharacter* Other = Cast<ACharacter>(result.GetActor()))
			{
				//플레이어 컨트롤러가 있다면
				if (Other->GetController()->IsPlayerController()) //이건 controller 부모함수에서 아마 Possess?된 플레이어면 true로 해주나봐 그럼 말은 어케됨
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, result.GetActor()); //타겟을 저장
					DrawDebugSphere(GetWorld(), Center, DetectRadius, 16, FColor::Green, false, 0.2f);

					DrawDebugPoint(GetWorld(), Other->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);//방향 표시

					DrawDebugLine(GetWorld(), Center, Other->GetActorLocation(), FColor::Cyan, false, 0.2f);
					return;
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr); //null값으로
	}

}
