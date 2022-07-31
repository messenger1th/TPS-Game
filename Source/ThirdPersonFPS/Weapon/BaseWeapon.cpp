// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	CurrentAmmo = DefaultAmmo;
}

void ABaseWeapon::MakeShot() {}

void ABaseWeapon::StartFire() {}

void ABaseWeapon::StopFire() {}

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}


AController* ABaseWeapon::GetController() const
{
	if (!GetWorld())return nullptr;

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController();
}

bool ABaseWeapon::SetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotator;
	if (!SetPlayerViewPoint(ViewLocation, ViewRotator)) return false; 

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotator.Vector();
	
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ABaseWeapon::MakeTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (GetWorld())	return;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
}

bool ABaseWeapon::SetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotator)
{
	const auto Controller = GetController();
	if (!Controller) return false;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotator);
	return true;
}

bool ABaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Bullets == 0;
}

bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Clips == 0;
}

void ABaseWeapon::ChangeClip()
{
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	--CurrentAmmo.Clips;
}

void ABaseWeapon::DecreaseAmmo()
{
	--CurrentAmmo.Bullets;
	LogAmmoInfromation();
	if (CurrentAmmo.Bullets == 0 && !IsClipEmpty())
	{
		ChangeClip();
	}
}

void ABaseWeapon::LogAmmoInfromation() const
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AmmoInfo);
}