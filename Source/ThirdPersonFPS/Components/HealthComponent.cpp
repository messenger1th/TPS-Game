// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	}
}

void UHealthComponent::OnTakeAnyDamageHandle(
	AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead())
		return;
	SetHealth(Health - Damage);
	if (IsDead())
	{
		OnDeath.Broadcast();		
	}
}

void UHealthComponent::SetHealth(float Value)
{
	if (Value == Health)
		return;
	Health = FMath::Clamp(Value, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
