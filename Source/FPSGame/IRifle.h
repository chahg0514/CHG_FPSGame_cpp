// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRifle.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRifle : public UInterface //인터페이스는 클래스가 2개, 위에꺼(이거)는 건드리면 안됨.
{
	GENERATED_BODY()
		//리플렉션 시스템이 (아마 언리얼브라우저, 가비지컬렉션등?)노출되도록 해주는 비어있는 클래스
};

/**
 * 
 */
class FPSGAME_API IIRifle
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class ACRifle* GetRifle() = 0;
	virtual void OnFocus() = 0;
	virtual void OffFocus() = 0;
	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) = 0;
};
