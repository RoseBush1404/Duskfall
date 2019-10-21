// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RangedWeapon.generated.h"

class ADuskfallProjectile;

/**
 * 
 */
UCLASS()
class DUSKFALL_API ARangedWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void HitEffect() override;

	UPROPERTY(EditDefaultsOnly, Category = "Ranged Weapon Stats")
		TSubclassOf<ADuskfallProjectile> Projectile;

};
