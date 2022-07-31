// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"
#include "ThirdPersonFPS/Other/EquipFinishedAnimNotify.h"
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

	// ...
	InitAnimation();
	SpawnWeapons();	
}



void UWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UWeaponComponent::ChangeHoldWeaponState()
{
	if (!ArmoryWeapons)
		return;
	
	if (CurrentWeapon == nullptr)
	{
		PlayEquipAnimMotage();
		PickUpCurrentWeaponFromArmoryWeapons();
	} else
	{
		//TODO set hold gun animation
		InsertCurrentWeaponToArmoryWeapons();
	}
	SetWeaponOutlook();
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

	for (auto WeaponClass: WeaponClasses)
	{
		const auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
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

	ClearWeaponOutlook();
	
	PlayEquipAnimMotage();

	InsertCurrentWeaponToArmoryWeapons();
	ArmoryWeapons = ArmoryWeapons->Next;
	PickUpCurrentWeaponFromArmoryWeapons();

	//TODO delete Debug Information;
	/**/
	UE_LOG(LogTemp, Warning, TEXT("Equip Next Weapon"));
	SetWeaponOutlook();
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (!CurrentWeapon || !CanEquip()) return;
	
	if (ArmoryWeapons == nullptr)	return; //No Remaining Weapon

	PlayEquipAnimMotage();
	
	ClearWeaponOutlook();
	
	InsertCurrentWeaponToArmoryWeapons();
	ArmoryWeapons = ArmoryWeapons->Previous;
	PickUpCurrentWeaponFromArmoryWeapons();

	//TODO delete Debug Information;
	/**/
	UE_LOG(LogTemp, Warning, TEXT("Equip Previous Weapon"));
	
	SetWeaponOutlook();
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
}

void UWeaponComponent::PickUpCurrentWeaponFromArmoryWeapons()
{
	CurrentWeapon = GetWeaponFromArmory(ArmoryWeapons);
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

void UWeaponComponent::PlayEquipAnimMotage()
{
	PlayAnimMontage(EquipAnimMontage);
	EquipAnimationProcessing = true;
}

void UWeaponComponent::InitAnimation()
{
	const auto NotifyEvents = EquipAnimMontage->Notifies;
	for (const auto& Event: NotifyEvents)
	{
		const auto EquipFinishedNotify = Cast<UEquipFinishedAnimNotify>(Event.Notify);
		if (EquipFinishedNotify)
		{
			EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
			break;
		}
	}
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	
	EquipAnimationProcessing = false;
	UE_LOG(LogTemp, Error, TEXT("Equip Finished"));
}

bool UWeaponComponent::CanFire()
{
	return CurrentWeapon != nullptr && !EquipAnimationProcessing;
}

bool UWeaponComponent::CanEquip()
{
	return !EquipAnimationProcessing;
}

