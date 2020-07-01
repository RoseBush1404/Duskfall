// Duskfall - Michael Rose - r015676g


#include "AdvDuskfallCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Components/Equipment/Equipment.h"
#include "Components/Equipment/EquipmentManager.h"

// Sets default values
AAdvDuskfallCharacter::AAdvDuskfallCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Set up audio components
	MovementAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementAudioComponent"));
	MovementAudioComponent->SetupAttachment(GetCapsuleComponent());

	//set up equipment manager
	EquipmentManager = CreateDefaultSubobject<UEquipmentManager>(TEXT("EquipmentManager"));

	//base values
	BaseTurnRate = 45.0f;
}

// Called when the game starts or when spawned
void AAdvDuskfallCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

#pragma region Character Getters Interface

UCharacterMovementComponent* AAdvDuskfallCharacter::GetCharacterMovementComponent_Implementation()
{
	return GetCharacterMovement();
}

APlayerCameraManager* AAdvDuskfallCharacter::GetPlayerCameraManager_Implementation()
{
	return nullptr;
}

#pragma endregion contains all charatcer getters implmentation

#pragma region Character Controls Interface

void AAdvDuskfallCharacter::JumpPressed_Implementation()
{
	if (JumpAudio != nullptr && GetCharacterMovement()->IsMovingOnGround())
	{
		MovementAudioComponent->SetSound(JumpAudio);
		MovementAudioComponent->Play(0.0f);
	}
	Jump();
}

void AAdvDuskfallCharacter::JumpReleased_Implementation()
{
	StopJumping();
}

void AAdvDuskfallCharacter::AttackPressed_Implementation()
{
}

void AAdvDuskfallCharacter::AttackReleased_Implementation()
{
}

void AAdvDuskfallCharacter::BlockPressed_Implementation()
{
}

void AAdvDuskfallCharacter::BlockReleased_Implementation()
{
}

void AAdvDuskfallCharacter::DashPressed_Implementation()
{
}

void AAdvDuskfallCharacter::DashReleased_Implementation()
{
}

void AAdvDuskfallCharacter::UsePressed_Implementation()
{
}

void AAdvDuskfallCharacter::UseReleased_Implementation()
{
}

void AAdvDuskfallCharacter::MoveForward_Implementation(float Scale)
{
	CharacterMoveForward(Scale);
}

void AAdvDuskfallCharacter::MoveRight_Implementation(float Scale)
{
	CharacterMoveRight(Scale);
}

void AAdvDuskfallCharacter::TurnRate_Implementation(float Scale)
{
	AddControllerYawInput(Scale * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAdvDuskfallCharacter::Turn_Implementation(float Scale)
{
	AddControllerYawInput(Scale);
}

#pragma endregion cotnaisn character controls implmentations

#pragma region Character Movement

void AAdvDuskfallCharacter::CharacterMoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);

		//add audio
		if (WalkAudio != nullptr)
		{
			if (MovementAudioComponent->Sound != WalkAudio && !MovementAudioComponent->IsPlaying() && GetCharacterMovement()->IsMovingOnGround())
			{
				MovementAudioComponent->SetSound(WalkAudio);
				MovementAudioComponent->Play(0.0f);
			}
		}
	}

	if (GetCharacterMovement()->Velocity == FVector(0.0f, 0.0f, 0.0f) || !GetCharacterMovement()->IsMovingOnGround())
	{
		if (MovementAudioComponent->Sound == WalkAudio || !MovementAudioComponent->IsPlaying())
		{
			MovementAudioComponent->Stop();
			MovementAudioComponent->SetSound(nullptr);
		}
	}
}

void AAdvDuskfallCharacter::CharacterMoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		//add audio
		if (WalkAudio != nullptr)
		{
			if (MovementAudioComponent->Sound != WalkAudio && !MovementAudioComponent->IsPlaying() && GetCharacterMovement()->IsMovingOnGround())
			{
				MovementAudioComponent->SetSound(WalkAudio);
				MovementAudioComponent->Play(0.0f);
			}
		}
	}
}

#pragma endregion contains the functiosn for moving the character around

#pragma region Health System

void AAdvDuskfallCharacter::TakeDamage_Implementation(float Damage, float DamageMoifier, AActor* DamageCauser)
{
	if (DamageCauser == this) { return; }
	if (CharacterState == EAdvCharacterState::ECS_Died) { return; }

	CurrentHealth = CurrentHealth - (Damage * DamageMoifier);

	if (CurrentHealth <= 0)
	{
		Died();
	}
}

void AAdvDuskfallCharacter::GainHealth_Implementation(float HealthToGain, AActor* HealingCauser)
{
	CurrentHealth = CurrentHealth + HealthToGain;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}

void AAdvDuskfallCharacter::Died()
{
	UE_LOG(LogTemp, Warning, TEXT("Have died"));
	CharacterState = EAdvCharacterState::ECS_Died;
}

#pragma endregion contains health system implmentations + Died() Implmentation
