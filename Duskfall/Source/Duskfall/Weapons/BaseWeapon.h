// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "PaperFlipbook.h"
#include "BaseWeapon.generated.h"

UCLASS()
class DUSKFALL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttackPressed();

	virtual void AttackReleased();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Standard vairs */
	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon States")
		UPaperFlipbook* IdleWeaponAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon States")
		UPaperFlipbook* ChargingWeaponAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon States")
		UPaperFlipbook* ChargeAttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon States")
		TArray<UPaperFlipbook*> StandardAttackAnimations;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon States")
		float MovementSpeedReduction = 200.0f;

	AActor* Owner;


	/* Attack charging fun and vair*/
	virtual void AttackingCharging();

	float chargeAmount = 0.0f;

	FTimerHandle weaponChargingTimer;

};
