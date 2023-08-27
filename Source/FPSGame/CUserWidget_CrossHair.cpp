// Fill out your copyright notice in the Description page of Project Settings.


#include "CUserWidget_CrossHair.h"
#include "Components/Image.h"

void UCUserWidget_CrossHair::OnFocus()
{
	CrossHair->SetBrushTintColor(FLinearColor::Red);
}

void UCUserWidget_CrossHair::OffFocus()
{
	CrossHair->SetBrushTintColor(FLinearColor::Green);
}
