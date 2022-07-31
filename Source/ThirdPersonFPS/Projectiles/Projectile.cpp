// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetRootComponent(CollisionComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 3000.0f;
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	check(CollisionComponent);
	
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	SetLifeSpan(LifeSpan);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;
	check(GetOwner());
	check(GetOwner()->GetInstigatorController());

	MovementComponent->StopMovementImmediately();

	/* test Code */
	bool Applied = false;
	/* test Code */

	Applied = UGameplayStatics::ApplyRadialDamage(GetWorld(),
		MaxDamageAmount,
		GetActorLocation(),
		DamageRadius,
		UDamageType::StaticClass(),
		 {GetOwner()},
		 this,
		 GetOwner()->GetInstigatorController(),
		 DoFullDamage
	);

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

	/* test Code */
	if (Applied) UE_LOG(LogTemp, Warning, TEXT("damaged"));
	/* test Code */

	Destroy();
}

AController* AProjectile::GetController()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ?  Pawn->GetController() : nullptr;
}

