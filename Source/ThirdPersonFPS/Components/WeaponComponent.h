// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPersonFPS/Weapon/BaseWeapon.h"
#include "CircleList.h"
#include "WeaponComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UAnimMontage* ReloadAnimMontage;
};


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
	TArray<FWeaponData> WeaponData;
	
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
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetWeaponBullets() const { return CurrentWeapon ? CurrentWeapon->GetCurrentAmmoBullets() : -1; }

	UFUNCTION(BlueprintCallable)
	int32 GetWeaponClips() const { return CurrentWeapon ? CurrentWeapon->GetCurrentAmmoClips() : -1; }
	
	UFUNCTION(BlueprintCallable)
	bool GetWeaponIsInfinite() const { return CurrentWeapon ? CurrentWeapon->IsAmmoInfinite() : false; }

	UFUNCTION(BlueprintCallable)
	bool GetCurrentWeaponAmmoData(FAmmoData& Ammo);
	
	UPROPERTY()
	UAnimMontage* CurrentReloadAniMontage;
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsHoldingWeapon() const { return CurrentWeapon != nullptr;	}

	UFUNCTION(BlueprintCallable)
	bool CanFire() { return CurrentWeapon != nullptr && !EquipAnimationProcessing && !ReloadAnimationProcessing; }

	UFUNCTION(BlueprintCallable)
	bool CanEquip() const { return !EquipAnimationProcessing && !ReloadAnimationProcessing; };

	UFUNCTION(BlueprintCallable)
	bool CanReload() const { return !EquipAnimationProcessing && !ReloadAnimationProcessing && CurrentWeapon != nullptr && CurrentWeapon->CanReloadBullets(); }
	
public:	
	void StartFire();
	void StopFire();
	void Reload();
	void EquipNextWeapon();
	void EquipPreviousWeapon();
	void ChangeHoldWeaponState();

	
private:
	CircleList<ABaseWeapon*>* ArmoryWeapons;
	ABaseWeapon* CurrentWeapon;
	
	void SpawnWeapons();
	
	void SetWeaponOutlook();
	void ClearWeaponOutlook();

	void SetReloadAnimMontage();
	void BeforeWeaponChange();
	void AfterWeaponChange();

	
	void AttachWeaponToSocket(ABaseWeapon* WeaponPtr,const FName& SocketName);
	void DetachWeaponFromSocket(ABaseWeapon* WeaponPtr);
	
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	
	void InsertCurrentWeaponToArmoryWeapons();
	void PickUpCurrentWeaponFromArmoryWeapons();
	static ABaseWeapon* GetWeaponFromArmory(CircleList<ABaseWeapon*>*& CurrentWeaponIterator);
	
	ABaseWeapon* GetPreviousWeapon();
	ABaseWeapon* GetNextWeapon();

	bool EquipAnimationProcessing = false;
	bool ReloadAnimationProcessing = false;
	
	void PlayAnimMontage(UAnimMontage* Animation);
	void PlayEquipAnimMontage();
	void PlayReloadAnimMontage();
	
	void InitAnimation();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
	


private:
	
	//TODO understand it and optimize its time complexity O(m * n).
	template<typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation);
};


template<typename T>
T* UWeaponComponent::FindNotifyByClass(UAnimSequenceBase* Animation)
{
	{
		if (!Animation) return nullptr;
		
		const auto NotifyEvents = Animation->Notifies;
		for (const auto& Event: NotifyEvents)
		{
			auto AnimationNotify = Cast<T>(Event.Notify);
			if (AnimationNotify)
			{
				return AnimationNotify;
			}
		}
		return nullptr;
	}
} 
