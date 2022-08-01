// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HealthComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

DECLARE_MULTICAST_DELEGATE(FOnDeath) //use to notify the death.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float) // notify the HeathChange

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() { return CurrentHealth / MaxHealth; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() {return CurrentHealth <= 0.0f;}

	UFUNCTION(BlueprintCallable)
	float GetHealth() const {return CurrentHealth;}

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribution")
	float MaxHealth = 100.f;

	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* EventInstigator, AActor* DamageCauser);

private:
	
	float CurrentHealth;
	void SetHealth(float Value);
	
public:
	
};
