// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "CPlayer.h"
#include "CAIController.h"
#include "CEnemy.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack");
    bNotifyTick = true;
}
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    Enemy->PlayAttackMontage();
    
    return EBTNodeResult::InProgress; //행동 진행중

    


    //return EBTNodeResult::Type();
}

//틱 다시 복습, 여기선 원거리 공격을 계속 해야되기때문에 틱을 쓴듯
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    ACEnemy* Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr) return;

    UAnimInstance* Anim = Cast<UAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
    if (Anim == nullptr) return;

    //현재 몽타주 재생이 끝났다면
    if(Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}
