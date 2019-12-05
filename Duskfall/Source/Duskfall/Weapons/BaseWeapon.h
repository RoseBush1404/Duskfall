// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShake.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "BaseWeapon.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class ADuskfallCharacter;
class USoundCue;
class UAudioComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_NotAttacking UMETA(DisplayName = "NotAttacking"),
	EWS_WaitingForRelease UMETA(DisplayName = "WaitingForRelase"),
	EWS_WindingUp UMETA(DisplayName = "WindingUp"),
	EWS_Damage UMETA(DisplayName = "Damage"),
	EWS_WindingDown UMETA(DisplayName = "WindingDown"),
	EWS_Charging UMETA(DisplayName = "Charging")
};

UCLASS()
class DUSKFALL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	virtual void AttackPressed();

	virtual void AttackReleased();

	void StopAttack();

	void SetUser(ADuskfallCharacter* Character) { User = Character; }

	void SetMuzzlePoint(UArrowComponent* muzzlePoint, USceneComponent* muzzleAttactmentPoint) { Muzzle = muzzlePoint; MuzzleTurret = muzzleAttactmentPoint; }

	void SetFlipbook(UPaperFlipbookComponent* Flipbook);

	void SetDefaultMovementSpeed(float MovementSpeed) { DefaultMovementSpeed = MovementSpeed; }

	float GetAttackStaminaDrain() { return AttackStaminaDrain; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Standard vairs */
	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		UPaperFlipbook* IdleWeaponAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		UPaperFlipbook* ChargingWeaponAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		UPaperFlipbook* ChargeAttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		TArray<UPaperFlipbook*> StandardAttackAnimations;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		float BaseDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		float AttackMovementSpeed= 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		float AttackStaminaDrain = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Weapon Stats")
		float AttackWindUp = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Weapon Stats")
		float AttackWindDown = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Weapon Stats")
		float RemainAttackTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		float MinChargeAmount = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		float MaxChargeAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Weapon Stats")
		TSubclassOf<UCameraShake> AttackCameraShake;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Weapon Stats")
		EWeaponState WeaponState = EWeaponState::EWS_NotAttacking;

	UPaperFlipbookComponent* FlipbookComponent;

	ADuskfallCharacter* User;

	/* basic attack */
	virtual void AttackTriggered();

	virtual void HitEffect();

	virtual void WindDownTrigger();

	virtual void AttackFinished();

	FTimerHandle WindUpDelayTimer;
	FTimerHandle WindDownDelayTimer;
	FTimerHandle AttackFinishDelayTimer;

	int NextPlayedAttackIndex = 0;

	float DefaultMovementSpeed = 0.0f;

	UArrowComponent* Muzzle;
	USceneComponent* MuzzleTurret;

	/* Attack charging fun and vair*/
	virtual void AttackingCharging();

	void SetChargeModifier();

	float chargeAmount = 0.0f;

	float ChargeModifier = 0.0f;

	FTimerHandle weaponChargingTimer;

	//audio

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* AttackAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;
};
