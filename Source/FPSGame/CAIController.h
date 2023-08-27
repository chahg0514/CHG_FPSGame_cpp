// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BTree;
//public:
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* LaserTree;
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* ProjTree;
	UPROPERTY(EditAnywhere, Category = "AI")
		TArray<class AActor*> PatrolPoints;
	
public:
	int32 CurrentPatrolPoint = 0;
	FORCEINLINE TArray<class AActor*> GetPatrolPoints() const
	{
		return PatrolPoints;
	}
	
};
