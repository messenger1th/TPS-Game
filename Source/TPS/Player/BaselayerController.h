// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaselayerController.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONFPS_API ABaselayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

};
