// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"
#include "ThirdPersonFPS/AnimNotify/EquipFinishedAnimNotify.h"
#include "ThirdPersonFPS/AnimNotify/ReloadFinishedAnimNotify.h"
#include "ThirdPersonFPS/Weapon/BaseWeapon.h"
// Sets default values for this component's properties



UWeaponComponent::UWeaponComponent():
	ArmoryWeapons(nullptr),
	CurrentWeapon(nullptr),
	AttachmentRules(EAttachmentRule::SnapToTarget, false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitAnimation();
	SpawnWeapons();	
}



void UWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UWeaponComponent::Reload()
{
	if (!CanReload()) return;
	PlayReloadAnimMontage();
	CurrentWeapon->ReloadBullets();

	//TODO delete  test log.
	UE_LOG(LogTemp, Error, TEXT("Reload"));
}

void UWeaponComponent::ChangeHoldWeaponState()
{
	if (!ArmoryWeapons)
		return;
	
	if (CurrentWeapon == nullptr)
	{
		PlayEquipAnimMontage();
		PickUpCurrentWeaponFromArmoryWeapons();
	} else
	{
		//TODO set hold gun animation
		InsertCurrentWeaponToArmoryWeapons();
	}
	SetWeaponOutlook();
}


bool UWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if (CurrentWeapon == nullptr)
		return false;
	WeaponUIData = CurrentWeapon->GetUIData();
	return true;
}

bool UWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& Ammo)
{
	if (CurrentWeapon == nullptr)
		return false;
	Ammo = CurrentWeapon->GetCurrentAmmoData();
	return true;
}

void UWeaponComponent::StartFire()
{
	if (!CurrentWeapon || !CanFire())	return;
	CurrentWeapon->StartFire();
}

void UWeaponComponent::SpawnWeapons()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	for (auto OneWeaponData: WeaponData)
	{
		const auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon)
		{
			//TODO notify.
			continue;
		}
		Weapon->SetOwner(Character);
		if (ArmoryWeapons == nullptr)
			ArmoryWeapons = new CircleList<ABaseWeapon*>(Weapon);
		else
			ArmoryWeapons = ArmoryWeapons->insert(Weapon);
	}
	
	if (ArmoryWeapons == nullptr) return;
	check(ArmoryWeapons);
	PickUpCurrentWeaponFromArmoryWeapons();
	SetWeaponOutlook();
}

void UWeaponComponent::DetachWeaponFromSocket(ABaseWeapon* WeaponPtr)
{
	if (WeaponPtr == nullptr) return;
	WeaponPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void UWeaponComponent::EquipNextWeapon()
{
	if (!CurrentWeapon || !CanEquip()) return;
	
	if (ArmoryWeapons == nullptr) return; //No Remaining Weapon

	BeforeWeaponChange();
	
	ArmoryWeapons = ArmoryWeapons->Next;

	AfterWeaponChange();
	
	//TODO delete Debug Information;
	/**/
	UE_LOG(LogTemp, Warning, TEXT("Equip Next Weapon"));
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (!CurrentWeapon || !CanEquip()) return;
	
	if (ArmoryWeapons == nullptr)	return; //No Remaining Weapon

	BeforeWeaponChange();
	
	ArmoryWeapons = ArmoryWeapons->Previous;
	
	AfterWeaponChange();
	//TODO delete Debug Information;
	/**/
	UE_LOG(LogTemp, Warning, TEXT("Equip Previous Weapon"));
}

void UWeaponComponent::SetWeaponOutlook()
{
	
	AttachWeaponToSocket(CurrentWeapon, EquipWeaponSocketName);
	if (ArmoryWeapons == nullptr) return;
	
	AttachWeaponToSocket(ArmoryWeapons->Previous->Data, PreviousWeaponSocketName);
	if (ArmoryWeapons->OneElement()) return;
	AttachWeaponToSocket(ArmoryWeapons->Data, NextWeaponSocketName);
}

void UWeaponComponent::ClearWeaponOutlook()
{
	DetachWeaponFromSocket(CurrentWeapon);
	DetachWeaponFromSocket(GetPreviousWeapon());
	DetachWeaponFromSocket(GetNextWeapon());
}

void UWeaponComponent::SetReloadAnimMontage()
{
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&] (const FWeaponData& Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});

	CurrentReloadAniMontage = CurrentWeaponData->ReloadAnimMontage;
}

void UWeaponComponent::BeforeWeaponChange()
{
	PlayEquipAnimMontage();
	
	ClearWeaponOutlook();
	
	InsertCurrentWeaponToArmoryWeapons();
}

void UWeaponComponent::AfterWeaponChange()
{
	PickUpCurrentWeaponFromArmoryWeapons();
	
	SetWeaponOutlook();
}


void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* WeaponPtr, const FName& SocketName)
{
	if (WeaponPtr == nullptr)
	{
		return;
	}
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	WeaponPtr->AttachToComponent(Character->GetMesh(), AttachmentRules, SocketName);
}

void UWeaponComponent::InsertCurrentWeaponToArmoryWeapons()
{
	ArmoryWeapons = ArmoryWeapons->insert(CurrentWeapon);
	CurrentWeapon = nullptr;
	CurrentReloadAniMontage = nullptr;
}

void UWeaponComponent::PickUpCurrentWeaponFromArmoryWeapons()
{
	CurrentWeapon = GetWeaponFromArmory(ArmoryWeapons);
	SetReloadAnimMontage();
}

ABaseWeapon* UWeaponComponent::GetWeaponFromArmory(CircleList<ABaseWeapon*>*& CurrentWeaponIterator)
{
	if (CurrentWeaponIterator == nullptr)
		return nullptr;
	ABaseWeapon* Weapon = CurrentWeaponIterator->Data;
	CurrentWeaponIterator = CurrentWeaponIterator->erase();
	return Weapon;
}

ABaseWeapon* UWeaponComponent::GetPreviousWeapon()
{
	if (ArmoryWeapons == nullptr)
		return nullptr;
	return ArmoryWeapons->Previous->Data;
}

ABaseWeapon* UWeaponComponent::GetNextWeapon()
{
	if (ArmoryWeapons == nullptr) return nullptr;
	return ArmoryWeapons->Data;
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!GetOwner()) return ;
	
	Character->PlayAnimMontage(Animation);
}

void UWeaponComponent::PlayEquipAnimMontage()
{
	EquipAnimationProcessing = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UWeaponComponent::PlayReloadAnimMontage()
{
	ReloadAnimationProcessing = true;
	PlayAnimMontage(CurrentReloadAniMontage);
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	
	EquipAnimationProcessing = false;
	UE_LOG(LogTemp, Error, TEXT("Equip Finished"));
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	
	ReloadAnimationProcessing = false;
	UE_LOG(LogTemp, Error, TEXT("Reload Finished"));
}

void UWeaponComponent::InitAnimation()
{
	//find relevant notification from animation notify event to bind function.
	const auto EquipFinishedNotify = FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);

	//TODO set clear DEBUG information.
	checkf(EquipFinishedNotify, TEXT("Please Set Equip finished Notify, unless the weapon doesn't work.")); 

	if (EquipFinishedNotify)
	{
		//bind function.
		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Please Set Equip finished notify In AnimMontage."))
	}
	
	//TODO Optimize it : Time Complexity O(m * n); m : Weapon number, n : classes' notify number;

	//Actually bind number depends on the weapon you have, such as three weapons will create 3 times bind
	//and notify 3 times after every //TODO waiting fix;
	/*find relevant reload notify for each weapon in WeaponData.*/ 
	for (const auto& OneWeaponData: WeaponData)
	{
		const auto ReloadFinishedNotify = FindNotifyByClass<UReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		
		checkf(ReloadFinishedNotify, TEXT("Please Set Reload finished Notify, unless the weapon doesn't work.")); 
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogTemp, Error, TEXT("Please Set Reload finished notify In AnimMontage."))
			continue;
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
	}
}



