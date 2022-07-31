// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ThirdPersonFPS/Components//HealthComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextRenderComponent->SetupAttachment(GetRootComponent());

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseCharacter::OnHealthChanged);
}

// Called every frame

// Called to bind functionality to input
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ABaseCharacter::TurnAround);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::StopCrouching);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("ChangeHoldWeaponState", IE_Pressed, WeaponComponent, &UWeaponComponent::ChangeHoldWeaponState);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, WeaponComponent, &UWeaponComponent::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UWeaponComponent::EquipNextWeapon);
}

void ABaseCharacter::TurnAround(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


void ABaseCharacter::MoveForward(float Value)
{
	// 找出"前进"方向，并记录玩家想向该方向移动。
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	// 找出"右侧"方向，并记录玩家想向该方向移动。
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}



void ABaseCharacter::Jump()
{
	Super::Jump();
}

void ABaseCharacter::StopJumping()
{
	Super::StopJumping();
}

void ABaseCharacter::StartCrouching()
{
	Super::Crouch();
}

void ABaseCharacter::StopCrouching()
{
	Super::UnCrouch();
}

float ABaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0.0f;
	}
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto ForwardNormal = GetActorForwardVector();
	const auto Angle = FMath::Acos(FVector::DotProduct(ForwardNormal, VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(ForwardNormal, VelocityNormal);
	const auto Degree = FMath::RadiansToDegrees(Angle);
	return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void ABaseCharacter::OnDeath() 
{
	UE_LOG(LogTemp, Warning, TEXT("Player %s is dead!"), *GetName());
	GetCharacterMovement()->DisableMovement();
	PlayAnimMontage(DeathAnimMontage);
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}
 
void ABaseCharacter::OnHealthChanged(float Health) const 
{
	HealthTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}


