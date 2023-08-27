// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UMyBTService_Detect : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyBTService_Detect();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	float DetectRadius = 600.f; //감지범위
	
};
