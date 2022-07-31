// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonFPS/Components//HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ThirdPersonFPS/Components/WeaponComponent.h"
#include "ThirdPersonFPS/Weapon/BaseWeapon.h"
#include "BaseCharacter.generated.h"

UCLASS()
class THIRDPERSONFPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,  BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere,  BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,  BlueprintReadWrite, Category="Components")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UTextRenderComponent* HealthTextRenderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	UAnimMontage* DeathAnimMontage;
	
	
	
	UPROPERTY(EditAnywhere)
	float Velocity;

	UFUNCTION(BlueprintCallable)
	bool GetIsCrouching() const {return bIsCrouched;};
	
	virtual void Jump() override;
	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable)
	float GetMovementDirection() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsHoldingWeapon() const { return WeaponComponent ->IsHoldingWeapon();};
	
private:
	void MoveForward(float value);
	void MoveRight(float value);
	void StartCrouching();
	void StopCrouching();
	void LookUp(float value);
	void TurnAround(float value);
	void OnDeath();
	void OnHealthChanged(float Health) const;
};
