// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "CPlayer.h"
#include "Blueprint/UserWidget.h"

ACGameMode::ACGameMode()
{
	DefaultPawnClass = ACPlayer::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> MinimapAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Player/WB_Minimap.WB_Minimap_C'"));
	if (MinimapAsset.Succeeded())
	{
		MinimapClass = MinimapAsset.Class;
	}
}

void ACGameMode::BeginPlay()
{
	Minimap = CreateWidget<UUserWidget>(GetWorld(), MinimapClass); //困连 积己
	Minimap->AddToViewport();
		//CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass); //困连 积己
}
