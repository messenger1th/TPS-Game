// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "ThirdPersonFPS/Projectiles/Projectile.h"
#include "LauncherWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONFPS_API ALauncherWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void MakeShot() override;;
	virtual void StartFire() override;;
	virtual void StopFire() override;
};
