// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UTexture2D* ThumbnailPreview;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category="UI")
	UTexture2D* CrossIcon;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta = (EditCondition = "!Infinite"));
	int32 SpareBullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool Infinite;
};


UCLASS()
class THIRDPERSONFPS_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FWeaponUIData GetUIData() const { return UIData; }

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmoBullets() const { return CurrentAmmo.Bullets; };

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmoClips() const { return CurrentAmmo.SpareBullets; }

	UFUNCTION(BlueprintCallable)
	bool IsAmmoInfinite() const { return CurrentAmmo.Infinite; }

	UFUNCTION(BlueprintCallable)
	FAmmoData GetCurrentAmmoData() const { return CurrentAmmo; }
	
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName MuzzleName = "MuzzleSocket";
	
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FAmmoData DefaultAmmo{30, 60, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float FirePeriod = 0.1f;
	
public:
	virtual void MakeShot();
	
	void StartFire();
	void StopFire();

	bool IsAmmoEmpty() const;
	bool IsSpareBulletsEmpty() const;
	
	void ReloadBullets();
	void DecreaseAmmo();

	bool CanReloadBullets() const { return CurrentAmmo.Infinite || (CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.SpareBullets > 0); }
	
	void LogAmmoInformation() const;

	
private:
	float TraceMaxDistance = 1500.0f;
	bool IsFiring = false;
	
protected:
	FVector GetMuzzleWorldLocation() const{
		return WeaponMesh->GetSocketLocation(MuzzleName);
	}

	AController* GetController() const;
	bool SetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotator);
	bool SetTraceData(FVector& TraceStart, FVector& TraceEnd);

	void MakeTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
private:
	FAmmoData CurrentAmmo;
	FTimerHandle FireDartleHandle;
	FTimerHandle FireGapHandle;
};
