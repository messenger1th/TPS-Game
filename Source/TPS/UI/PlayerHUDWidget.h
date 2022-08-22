// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThirdPersonFPS/Components/HealthComponent.h"
#include "ThirdPersonFPS/Components/WeaponComponent.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONFPS_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable, Category= "UI")
	bool GetCurrentAmmoData(FAmmoData& Ammo);

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData);

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsHoldingWeapon() const;
	
protected:

	template<typename T>
	T* GetComponentFromPlayer() const;
};


template <typename T>
T* UPlayerHUDWidget::GetComponentFromPlayer() const
{
	const auto Player = GetOwningPlayerPawn();
	
	return Player == nullptr ? nullptr : Cast<T>(Player->GetComponentByClass(T::StaticClass()));
}

