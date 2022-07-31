// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta = (EditCondition = "!Infinite"));
	int32 Clips;

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
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName MuzzleName = "MuzzleSocket";
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FAmmoData DefaultAmmo{30, 5, false};

	virtual void MakeShot();
	virtual void StartFire();
	virtual void StopFire();

	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void ChangeClip();
	void DecreaseAmmo();

	void LogAmmoInfromation() const;
private:
	float TraceMaxDistance = 1500;

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
};
