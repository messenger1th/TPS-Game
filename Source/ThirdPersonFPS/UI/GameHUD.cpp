// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"


void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	const auto PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);

	check(PlayerWidget);

	PlayerWidget->AddToViewport();
}

