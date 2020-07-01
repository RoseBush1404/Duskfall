// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterControls.h"
#include "Characters/AdvCharacterGetters.h"
#include "Characters/HealthSystem.h"
#include "AdvDuskfallCharacter.generated.h"

class USoundCue;
class UAudioComponent;
class UEquipmentManager;

UENUM()
enum class EAdvCharacterState
{
	ECS_Moveable UMETA(DisplayName = "Moveable"),
	ECS_Died UMETA(DisplayName = "Died"),
	ECS_Staggered UMETA(DisplayName = "Starggered"),
	ECS_Parryed UMETA(DisplayName = "Parryed"),
	ECS_UsingItem UMETA(DisplayName = "Using Item")
};

UCLASS()
class DUSKFALL_API AAdvDuskfallCharacter : public ACharacter, public ICharacterControls, public IAdvCharacterGetters, public IHealthSystem
{
	GENERATED_BODY()
		
//values
public:

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
		TEnumAsByte<EAdvCharacterState> CharacterState = EAdvCharacterState::ECS_Moveable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
		UEquipmentManager* EquipmentManager;

#pragma region Movement based Audio

	UAudioComponent* MovementAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* WalkAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* JumpAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* DashAudio;

#pragma endregion contains audio for movement

#pragma region Health

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100;

	float CurrentHealth;

#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Settings")
		float BaseTurnRate;

private:

//funcations
public:
	
	AAdvDuskfallCharacter();

#pragma region Character Controls Interface
	
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

#pragma endregion contains all character controls delcrations

#pragma region Character Getters Interface

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
		UCharacterMovementComponent* GetCharacterMovementComponent();
	virtual UCharacterMovementComponent* GetCharacterMovementComponent_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
		APlayerCameraManager* GetPlayerCameraManager();
	virtual APlayerCameraManager* GetPlayerCameraManager_Implementation() override;

#pragma endregion contains all the character getter delcrations

#pragma region Health System Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthSystem")
		void TakeDamage(float Damage, float DamageMoifier, AActor * DamageCauser);
	virtual void TakeDamage_Implementation(float Damage, float DamageMoifier, AActor * DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthSystem")
		void GainHealth(float HealthToGain, AActor * HealingCauser);
	virtual void GainHealth_Implementation(float HealthToGain, AActor * HealingCauser) override;

#pragma endregion contains all the health system delcrations

protected:
	
	virtual void BeginPlay() override;

	void CharacterMoveForward(float value);
	void CharacterMoveRight(float Value);

	virtual void Died();

private:	


};
