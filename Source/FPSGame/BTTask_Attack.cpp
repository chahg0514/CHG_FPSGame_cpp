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
    if (Target == nullptr) return EBTNodeResult::Failed; //�̰� �ʿ���� �ʳ� ������ ���ڷ����Ϳ� �ƴϸ� �׳� ������� �迡 ���ִ°ǰ�

    ACEnemy* Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr) return EBTNodeResult::Failed;

    FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation(); //Ÿ�������� ����
    LookVector.Z = 0.f; //���̴� �ʿ����.
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //������ ȸ�������� ��ȯ X�� ������ ȸ�������� ��ȯ
    Enemy->SetActorRotation(TargetRot);
    Enemy->PlayAttackMontage();
    
    return EBTNodeResult::InProgress; //�ൿ ������

    


    //return EBTNodeResult::Type();
}

//ƽ �ٽ� ����, ���⼱ ���Ÿ� ������ ��� �ؾߵǱ⶧���� ƽ�� ����
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    ACEnemy* Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr) return;

    UAnimInstance* Anim = Cast<UAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
    if (Anim == nullptr) return;

    //���� ��Ÿ�� ����� �����ٸ�
    if(Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}
