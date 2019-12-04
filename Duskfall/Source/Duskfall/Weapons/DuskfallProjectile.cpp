// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../Characters/HealthSystem.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

ADuskfallProjectile::ADuskfallProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADuskfallProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
}

void ADuskfallProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	IHealthSystem* HealthSystem = Cast<IHealthSystem>(OtherActor);
	if (HealthSystem != nullptr) // C++ Layer
	{
		HealthSystem->Execute_TakeDamage(OtherActor, DamageAmount, DamageModifer, this);
		if (HitAudio != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
		}
	}
	else if (OtherActor != nullptr && OtherActor->GetClass() != nullptr && OtherActor->GetClass()->ImplementsInterface(UHealthSystem::StaticClass())) // Blueprint Layer
	{
		IHealthSystem::Execute_TakeDamage(OtherActor, DamageAmount, DamageModifer, this);
		if (HitAudio != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
		}
	}

	Destroy();
}