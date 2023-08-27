// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget)) //Blueprint 랑 연결을 하냐 이건가봐
		class UImage* CrossHair;
public:
	void OnFocus();
	void OffFocus();
};
