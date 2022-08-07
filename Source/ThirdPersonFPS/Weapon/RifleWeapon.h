// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONFPS_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 10.0f;

public:
	ARifleWeapon();
	virtual void MakeShot() override;

private:
	void MakeDamage(FHitResult& HitResult);
};
