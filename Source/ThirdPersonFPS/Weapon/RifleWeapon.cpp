// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"



void ARifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;
	
	FVector TraceStart, TraceEnd;
	if (!SetTraceData(TraceStart, TraceEnd))	return;
	
	FHitResult HitResult;
	MakeTraceHit(HitResult, TraceStart, TraceEnd);
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(),  HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Blue, false, 3.0f);
		
	} else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(),  TraceEnd, FColor::Green, false, 3.0f, 0, 3.0f);
	}
	DecreaseAmmo();
}

void ARifleWeapon::StartFire()
{
	
	MakeShot();
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ARifleWeapon::MakeShot, ShootPeriod, true);
}

void ARifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ARifleWeapon::MakeDamage(FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

