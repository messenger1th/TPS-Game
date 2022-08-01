// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimNotify.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);
/**
 * 
 */
UCLASS()
class THIRDPERSONFPS_API UEquipFinishedAnimNotify : public UBaseAnimNotify
{
	GENERATED_BODY()

public:

};


