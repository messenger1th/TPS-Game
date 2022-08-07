// Fill out your copyright notice in the Description page of Project Settings.


#include "LauncherWeapon.h"

#include "Kismet/GameplayStatics.h"

ALauncherWeapon::ALauncherWeapon()
{
	FirePeriod = 1.0;
}

void ALauncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;

	Super::MakeShot();
	UE_LOG(LogTemp, Error, TEXT("lAUCNH"));

	FVector TraceStart, TraceEnd;
	if (!SetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeTraceHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
	
	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, SpawnTransform);
	//set Projectile parameters.
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
}

