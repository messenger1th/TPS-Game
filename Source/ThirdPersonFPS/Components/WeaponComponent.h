// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPersonFPS/Weapon/BaseWeapon.h"
#include "CircleList.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONFPS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName EquipWeaponSocketName = "EquipWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName NextWeaponSocketName = "LeftWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName PreviousWeaponSocketName = "RightWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category="Animaiton")
	UAnimMontage* EquipAnimMontage;

public:
	UFUNCTION(BlueprintCallable)
	bool IsHoldingWeapon() const { return CurrentWeapon != nullptr;	}

	
public:	
	void StartFire();
	void StopFire();

	void EquipNextWeapon();
	void EquipPreviousWeapon();
	void ChangeHoldWeaponState();


	
private:
	CircleList<ABaseWeapon*>* ArmoryWeapons;
	ABaseWeapon* CurrentWeapon;
	
	void SpawnWeapons();
	
	void SetWeaponOutlook();
	void ClearWeaponOutlook();
	
	void AttachWeaponToSocket(ABaseWeapon* WeaponPtr,const FName& SocketName);
	void DetachWeaponFromSocket(ABaseWeapon* WeaponPtr);
	
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	
	void InsertCurrentWeaponToArmoryWeapons();
	void PickUpCurrentWeaponFromArmoryWeapons();
	static ABaseWeapon* GetWeaponFromArmory(CircleList<ABaseWeapon*>*& CurrentWeaponIterator);
	
	ABaseWeapon* GetPreviousWeapon();
	ABaseWeapon* GetNextWeapon();

	bool EquipAnimationProcessing = false;
	
	void PlayAnimMontage(UAnimMontage* Animation);
	void PlayEquipAnimMotage();
	
	void InitAnimation();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	
	
	bool CanFire();
	bool CanEquip();
	
};

