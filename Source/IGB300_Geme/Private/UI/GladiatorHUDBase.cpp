// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GladiatorHUDBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/GladiatorAttributeWidget.h"

void AGladiatorHUDBase::Init()
{
	AttributeWidget = CreateWidget<UGladiatorAttributeWidget>(GetOwningPlayerController(), AttributeWidgetClass); //creating a new widget
	AttributeWidget->BindToAttributes();//binding the attributes to the widget 
	AttributeWidget->AddToViewport(); //adds to the view port of the player

	GloryAttributeWidget = CreateWidget<UGloryAttributeWidget>(GetOwningPlayerController(), GloryAttributeWidgetClass);
	GloryAttributeWidget->BindAttributes();
	GloryAttributeWidget->AddToViewport();
	
}

void AGladiatorHUDBase::Death()
{
	DeathWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), DeathUIClass);
	DeathWidget->AddToViewport();
}
