// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "ThirdPersonFPS/Components/WeaponComponent.h"
#include "ThirdPersonFPS/Other/Utils.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	check(WeaponMesh);
	
	//TODO checkf usage.
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero")); 
	checkf(DefaultAmmo.Bullets >= 0, TEXT("Bullets count couldn't be less  zero"));
	
	CurrentAmmo = DefaultAmmo;
}

void ABaseWeapon::MakeShot() {} //UE can't declare pure virtual function.

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
	if (!GetWorld()) return;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
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
	return CurrentAmmo.Bullets == 0;
}

bool ABaseWeapon::IsSpareBulletsEmpty() const
{
	return CurrentAmmo.SpareBullets == 0;
}

void ABaseWeapon::ReloadBullets()
{
	if (!CanReloadBullets())
		return;
	if (CurrentAmmo.Infinite)
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	} else
	{
		const int32 AddBullets = std::min(DefaultAmmo.Bullets - CurrentAmmo.Bullets, CurrentAmmo.SpareBullets);
		CurrentAmmo.SpareBullets -= AddBullets;
		CurrentAmmo.Bullets += AddBullets;
	}
}

void ABaseWeapon::DecreaseAmmo()
{
	--CurrentAmmo.Bullets;
	LogAmmoInformation();
}

void ABaseWeapon::LogAmmoInformation() const
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.SpareBullets);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AmmoInfo);
}