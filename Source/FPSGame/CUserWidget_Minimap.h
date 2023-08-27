// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Minimap.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UCUserWidget_Minimap : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minimap", meta = (BindWidget))
		class UImage* MinimapImage;
	
};
