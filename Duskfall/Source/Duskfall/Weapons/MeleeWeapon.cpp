// Duskfall - Michael Rose - r015676g


#include "MeleeWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "../Characters/HealthSystem.h"
#include "../Characters/DuskfallCharacter.h"

void AMeleeWeapon::HitEffect()
{
	if (Muzzle != nullptr)
	{
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(HitBoxHalfSize);
		FVector StartLocation = Muzzle->GetComponentLocation();
		FVector EndLocation = Muzzle->GetComponentLocation() + Muzzle->GetForwardVector();
		FRotator Rotator = Muzzle->GetComponentRotation();
		FQuat ForwardRotation = Rotator.Quaternion();

		TArray<FHitResult> OutResults;
		bool isHit = GetWorld()->SweepMultiByChannel(OutResults, StartLocation, EndLocation, ForwardRotation, ECollisionChannel::ECC_Visibility, CollisionShape);
		DrawDebugBox(GetWorld(), StartLocation, HitBoxHalfSize, ForwardRotation, FColor::Red, true, 5.0f);

		if (isHit)
		{
			for (auto& Hit : OutResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("in for loop"));
				// C++ Layer
				IHealthSystem* HealthSystem = Cast<IHealthSystem>(Hit.GetActor());
				if (HealthSystem != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("got health system"));
					HealthSystem->Execute_TakeDamage(Hit.GetActor(), BaseDamage, ChargeModifier, Cast<AActor>(User));
				}
				else if(Hit.GetActor()->GetClass()->ImplementsInterface(UHealthSystem::StaticClass())) // Blueprint Layer
				{
					IHealthSystem::Execute_TakeDamage(Hit.GetActor(), BaseDamage, ChargeModifier, Cast<AActor>(User));
				}

				UE_LOG(LogTemp, Warning, TEXT("actor hit: %s"), *Hit.GetActor()->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("no hit"));
		}
	}

	Super::HitEffect();
}
