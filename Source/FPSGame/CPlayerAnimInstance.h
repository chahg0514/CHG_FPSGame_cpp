// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UCPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool isEquipped;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool isAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool isAiming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Pitch;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class ACPlayer* OwnerCharacter;
	
};
