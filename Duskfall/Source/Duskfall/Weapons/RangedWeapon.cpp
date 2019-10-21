// Duskfall - Michael Rose - r015676g


#include "RangedWeapon.h"
#include "Engine/World.h"
#include "Weapons/DuskfallProjectile.h"
#include "../Characters/DuskfallCharacter.h"

void ARangedWeapon::HitEffect()
{
	FActorSpawnParameters SpawnInfo;
	ADuskfallProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ADuskfallProjectile>(Projectile, Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation(), SpawnInfo);
	SpawnedProjectile->DamageAmount = BaseDamage;
	SpawnedProjectile->DamageModifer = ChargeModifier;
	SpawnedProjectile->SetOwner(User);

	Super::HitEffect();
}