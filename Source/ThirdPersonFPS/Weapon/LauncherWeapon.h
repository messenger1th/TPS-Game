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
	ALauncherWeapon();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AProjectile> ProjectileClass;
	
protected:
	
	virtual void MakeShot() override;
};
