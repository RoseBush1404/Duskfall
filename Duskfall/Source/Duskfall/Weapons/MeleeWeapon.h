// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Engine/Public/WorldCollision.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DUSKFALL_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void HitEffect() override;

	virtual void AttackTriggered() override;

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		FVector HitBoxHalfSize = FVector(100.0f,50.0f,50.0f);

};
