// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Components/Equipment/Weapons/AdvBaseWeapon.h"
#include "AdvMeleeWeapon.generated.h"

/**
 * 
 */

class USoundCue;

UENUM()
enum EAdvWeaponState
{
	AWS_Idle   UMETA(DisplayName = "Idle"),
	AWS_WindingUp     UMETA(DisplayName = "WindingUp"),
	AWS_Damage   UMETA(DisplayName = "Damage"),
	AWS_WindingDown UMETA(DisplayName = "WindingDown"),
	AWS_Reset UMETA(DisplayName = "Reset"),
	AWS_Charging UMETA(DisplayName = "Charging")
};

UCLASS()
class DUSKFALL_API AAdvMeleeWeapon : public AAdvBaseWeapon
{
	GENERATED_BODY()

//values
public:

protected:

#pragma region Animations

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		TArray<USpriteAnimConfig*> AttackAnim;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* AttackResetAnim;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* AttackChargingAnim;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* ChargeAttackAnim;

#pragma endregion Contains all the animations for the melee weapon

#pragma region Standard Melee Weapon Values

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		TEnumAsByte<EAdvWeaponState> WeaponState = EAdvWeaponState::AWS_Idle;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Effects")
		TSubclassOf<UCameraShake> AttackCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		float BaseDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		FVector HitBoxHalfSize = FVector(100.0f, 50.0f, 50.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		FVector HitBoxOffSet = FVector(100.0f, 0.0f, 35.0f);

	int CurrentComboCount = 0;

	bool HasStackedInput = false;

#pragma endregion contains state, damage, Hitbox, and stacked input

#pragma region Audio

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* AttackAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;

#pragma endregion contains the audio values

	FTimerHandle ChargeAttackTimer;

	float AttackModifer = 1.0f;

private:


//functions
public:

	virtual void UsePressed() override;

	virtual void UseReleased() override;

	virtual void AnimEventHit_Implementation(FString& AnimMessage) override;

protected:

	void HitScan();

	void DoChargeAttack();

private:

	void PlayNextAttackAnim();

	void TriggerAttackEffects();

	void AttackCharging();

#pragma region Animation Events

	void AttackFullyCharged();

	void AttackHalfCharged();

	void AttackResetFinishedEvent();

	void AttackFinishedEvent();

	void WindingDownEvent();

	void HitEvent();

#pragma endregion
};
