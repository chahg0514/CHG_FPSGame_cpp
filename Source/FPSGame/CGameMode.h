// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACGameMode();
private:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
		TSubclassOf<class UUserWidget> MinimapClass;
	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
		class UUserWidget* Minimap;
};
