// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterControls.h"
#include "Characters/HealthSystem.h"
#include "Characters/CharacterGetters.h"
#include "Camera/CameraShake.h"
#include "DuskfallCharacter.generated.h"

class ABaseWeapon;
class ABaseShield;
class USoundCue;
class UAudioComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Moveable UMETA(DisplayName = "Moveable"),
	ECS_Died UMETA(DisplayName = "Died"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_Blocking UMETA(DisplayName = "Blocking"),
	ECS_Staggered UMETA(DisplayName = "Starggered"),
	ECS_Parrying UMETA(DisplayName = "Parrying"),
	ECS_Parryed UMETA(DisplayName = "Parryed"),
	ECS_UsingItem UMETA(DisplayName = "Using Item")
};


UCLASS(config=Game)
class ADuskfallCharacter : public ACharacter, public ICharacterControls, public IHealthSystem, public ICharacterGetters
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Muzzle, meta = (AllowPrivateAccess = "true"))
			UArrowComponent* MuzzlePoint;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Muzzle, meta = (AllowPrivateAccess = "true"))
			USceneComponent* MuzzleAttachmentPoint;

public:
	ADuskfallCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		float CurrentStamina = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		float CurrentHealth = 0.0f;

	ECharacterState GetCharacterState() { return CharacterState; }
	void UpdateCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	void UpdateMuzzleRotation(FRotator NewRotation);

	virtual void CharacterStaggered();
	virtual void EndStagger();
	virtual void CharacterParryed();

	virtual void RegenStamina();
	void DecreaseStamina(float StaminaToLose) { CurrentStamina = CurrentStamina - StaminaToLose; }

	virtual void PlayGivenCameraShake(TSubclassOf<UCameraShake> GivenCameraShake, float Scale);

	/* Health System Interface */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Sytem")
		void TakeDamage(float Damage, float DamageMoifier, AActor* DamageCauser);
		virtual void TakeDamage_Implementation(float Damage, float DamageMoifier, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Sytem")
		void GainHealth(float HealthToGain, AActor* HealingCauser);
		virtual void GainHealth_Implementation(float HealthToGain, AActor* HealingCauser) override;
	/* End of Health System Interface */

	/* Character Getters Interface */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
		ABaseWeapon* GetWeapon();
		virtual ABaseWeapon* GetWeapon_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
		UCharacterMovementComponent* GetCharacterMovementComponent();
		virtual UCharacterMovementComponent* GetCharacterMovementComponent_Implementation() override;
	/* End of Character Getters Interface*/

	/* Character control interface */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void JumpPressed();
		virtual void JumpPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void JumpReleased();
		virtual void JumpReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void AttackPressed();
		virtual void AttackPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void AttackReleased();
		virtual void AttackReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void BlockPressed();
		virtual void BlockPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void BlockReleased();
		virtual void BlockReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void DashPressed();
		virtual void DashPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void DashReleased();
		virtual void DashReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void UsePressed();
		virtual void UsePressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void UseReleased();
		virtual void UseReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void MoveForward(float Scale);
		virtual void MoveForward_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void MoveRight(float Scale);
		virtual void MoveRight_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void TurnRate(float Scale);
		virtual void TurnRate_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void Turn(float Scale);
		virtual void Turn_Implementation(float Scale) override;
	/* End of Character Controls Interface */
	
	//virtual void Die();

protected:
	virtual void BeginPlay();

	virtual void RemoveHealth(float Damage);

	virtual void Dash();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		ECharacterState CharacterState = ECharacterState::ECS_Moveable;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float MaxHealth = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float MaxStamina = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float StaminaRegenAmount = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float StaminaRegenRate = 0.1f;
	FTimerHandle RegenStaminaTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float DashVelocityModifier = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float DashUpwardForce = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float DashStaminaDrain = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float StaggerLength = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float ParryedLength = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<ABaseWeapon> StartingWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<ABaseShield> StartingShield;

	ABaseWeapon* Weapon;
	ABaseShield* Shield;

	FTimerHandle StaggerDelayTimer;
	FTimerHandle ParryedDelayTimer;

	//Audio

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* WalkAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* JumpAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* DashAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* DeathAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* HitAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ParriedAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;

	UAudioComponent* MovementAudioComponent;
};

