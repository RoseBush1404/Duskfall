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
		//DrawDebugBox(GetWorld(), StartLocation, HitBoxHalfSize, ForwardRotation, FColor::Red, false, 5.0f);

		if (isHit)
		{
			for (auto& Hit : OutResults)
			{
				IHealthSystem* HealthSystem = Cast<IHealthSystem>(Hit.GetActor());
				if (HealthSystem != nullptr) // C++ Layer
				{
					HealthSystem->Execute_TakeDamage(Hit.GetActor(), BaseDamage, ChargeModifier, Cast<AActor>(User));
				}
				else if(Hit.GetActor() != nullptr && Hit.GetActor()->GetClass() != nullptr && Hit.GetActor()->GetClass()->ImplementsInterface(UHealthSystem::StaticClass())) // Blueprint Layer
				{
					IHealthSystem::Execute_TakeDamage(Hit.GetActor(), BaseDamage, ChargeModifier, Cast<AActor>(User));
				}
			}
		}
	}

	Super::HitEffect();
}
