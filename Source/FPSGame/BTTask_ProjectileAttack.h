// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UBTTask_ProjectileAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ProjectileAttack(); //생성자인가봐
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
