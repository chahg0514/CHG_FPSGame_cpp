// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ProjectileAttack.h"

#include "CPlayer.h"
#include "CAIController.h"
#include "CEnemy.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ProjectileAttack::UBTTask_ProjectileAttack()
{
	NodeName = TEXT("ProjectileAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ProjectileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACAIController* EnemyController = Cast<ACAIController>(OwnerComp.GetAIOwner());
    if (EnemyController == nullptr) return EBTNodeResult::Failed;
    auto Target = Cast<ACPlayer>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetKey));
    if (Target == nullptr) return EBTNodeResult::Failed; //이거 필요없지 않나 어차피 데코레이터에 아니면 그냥 갖고오는 김에 써주는건가

    ACEnemy* Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr) return EBTNodeResult::Failed;

    FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation(); //타겟으로의 방향
    LookVector.Z = 0.f; //높이는 필요없다.
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //방향을 회전값으로 변환 X축 기준의 회전값으로 변환
    Enemy->SetActorRotation(TargetRot);
    Enemy->PlayProjectileAttackMontage();
    return EBTNodeResult::InProgress;
}

void UBTTask_ProjectileAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
