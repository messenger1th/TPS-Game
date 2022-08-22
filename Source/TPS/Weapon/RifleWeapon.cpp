// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

ARifleWeapon::ARifleWeapon()
{
	FirePeriod = 0.1;
}

void ARifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;
	
	Super::MakeShot();

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


void ARifleWeapon::MakeDamage(FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

