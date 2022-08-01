// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

float UPlayerHUDWidget::GetHealthPercent()
{
	const auto HealthComponent = GetComponentFromPlayer<UHealthComponent>();
	return HealthComponent == nullptr ? 0.0f : HealthComponent->GetHealthPercent();
}

bool UPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& Ammo)
{
	const auto WeaponComponent = GetComponentFromPlayer<UWeaponComponent>();
	if (!WeaponComponent) return false;
	return WeaponComponent->GetCurrentWeaponAmmoData(Ammo);
}

bool UPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& WeaponUIData)
{
	const auto WeaponComponent = GetComponentFromPlayer<UWeaponComponent>();
	if (!WeaponComponent) return false;
	return WeaponComponent->GetWeaponUIData(WeaponUIData);
}

bool UPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetComponentFromPlayer<UHealthComponent>();
	return HealthComponent != nullptr && !HealthComponent->IsDead();
}

bool UPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller != nullptr && Controller->GetStateName() == NAME_Spectating;
}

bool UPlayerHUDWidget::IsHoldingWeapon() const
{
	const auto WeaponComponent = GetComponentFromPlayer<UWeaponComponent>();
	return WeaponComponent != nullptr && WeaponComponent->IsHoldingWeapon();
}
