// Duskfall - Michael Rose - r015676g


#include "RangedWeapon.h"
#include "Engine/World.h"
#include "Weapons/DuskfallProjectile.h"
#include "../Characters/DuskfallCharacter.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

void ARangedWeapon::HitEffect()
{
	FActorSpawnParameters SpawnInfo;
	ADuskfallProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ADuskfallProjectile>(Projectile, Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation(), SpawnInfo);
	SpawnedProjectile->DamageAmount = BaseDamage;
	SpawnedProjectile->DamageModifer = ChargeModifier;
	SpawnedProjectile->SetOwner(User);

	if (AttackAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}

	Super::HitEffect();
}