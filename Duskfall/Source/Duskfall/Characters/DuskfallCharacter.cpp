// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/InputSettings.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "../Weapons/BaseWeapon.h"
#include "../Weapons/BaseShield.h"
#include "GameFramework/CharacterMovementComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ADuskfallCharacter

ADuskfallCharacter::ADuskfallCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	MuzzleAttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleAttachmentPoint"));
	MuzzleAttachmentPoint->SetupAttachment(GetCapsuleComponent());

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(MuzzleAttachmentPoint);
	MuzzlePoint->SetRelativeLocation(FVector(110.0f, 0.0f, 35.0f));

	//Set up audio components
	MovementAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementAudioComponent"));
	MovementAudioComponent->SetupAttachment(GetCapsuleComponent());

	// set our turn rates for input
	BaseTurnRate = 45.0f;
}

void ADuskfallCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UCharacterMovementComponent* MovementComponent = this->GetCharacterMovement();

	if (StartingWeapon != nullptr)
	{
		Weapon = GetWorld()->SpawnActor<ABaseWeapon>(StartingWeapon, GetActorTransform());
		Weapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Weapon->SetUser(this);
		Weapon->SetMuzzlePoint(MuzzlePoint, MuzzleAttachmentPoint);
		Weapon->SetDefaultMovementSpeed(MovementComponent->MaxWalkSpeed);
	}
	if (StartingShield != nullptr)
	{
		Shield = GetWorld()->SpawnActor<ABaseShield>(StartingShield, GetActorTransform());
		Shield->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Shield->SetUser(this);
		Shield->SetDefaultMovementSpeed(MovementComponent->MaxWalkSpeed);
	}
	CurrentStamina = MaxStamina;
	CurrentHealth = MaxHealth;
}

void ADuskfallCharacter::EndStagger()
{
	UpdateCharacterState(ECharacterState::ECS_Moveable);
}

void ADuskfallCharacter::RemoveHealth(float Damage)
{
	CurrentHealth = CurrentHealth - Damage;

	if (HitAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}
}

void ADuskfallCharacter::Dash()
{
	//remove x amount of stamina
	CurrentStamina = CurrentStamina - DashStaminaDrain;

	//if have shield and using it to block then auto-lower shield
	if (Shield != nullptr && CharacterState == ECharacterState::ECS_Blocking) { Shield->DropShield(); }

	//add impule
	GetCharacterMovement()->AddImpulse(FVector(0.0f, 0.0f, DashUpwardForce), true);
	GetCharacterMovement()->AddImpulse(GetVelocity() * DashVelocityModifier, true);

	//play audio
	if (DashAudio != nullptr)
	{
		MovementAudioComponent->SetSound(DashAudio);
		MovementAudioComponent->Play(0.0f);
	}
}

void ADuskfallCharacter::UpdateMuzzleRotation(FRotator NewRotation)
{
	MuzzleAttachmentPoint->SetWorldRotation(FRotator(NewRotation.Pitch, NewRotation.Yaw, NewRotation.Roll));
}

void ADuskfallCharacter::CharacterStaggered()
{
}

void ADuskfallCharacter::CharacterParryed()
{
}

void ADuskfallCharacter::RegenStamina()
{
	switch (CharacterState)
	{
	case ECharacterState::ECS_Moveable:
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina = CurrentStamina + StaminaRegenAmount;
		}
		break;
	case ECharacterState::ECS_Died:
		break;
	case ECharacterState::ECS_Attacking:
		break;
	case ECharacterState::ECS_Blocking:
		break;
	case ECharacterState::ECS_Staggered:
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina = CurrentStamina + StaminaRegenAmount;
		}
		break;
	case ECharacterState::ECS_Parrying:
		break;
	case ECharacterState::ECS_UsingItem:
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina = CurrentStamina + StaminaRegenAmount;
		}
		break;
	default:
		break;
	}
	CurrentStamina = FMath::Clamp(CurrentStamina, -MaxStamina, MaxStamina);
}


void ADuskfallCharacter::PlayGivenCameraShake(TSubclassOf<UCameraShake> GivenCameraShake, float Scale)
{
}

/* Helath System Interface*/
void ADuskfallCharacter::TakeDamage_Implementation(float Damage, float DamageMoifier, AActor * DamageCauser)
{
}

void ADuskfallCharacter::GainHealth_Implementation(float HealthToGain, AActor * HealingCauser)
{
	CurrentHealth = CurrentHealth + HealthToGain;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}
/* End of Health System Interface */

/* Character Getters Interface*/
ABaseWeapon * ADuskfallCharacter::GetWeapon_Implementation()
{
	return Weapon;
}
UCharacterMovementComponent * ADuskfallCharacter::GetCharacterMovementComponent_Implementation()
{
	return GetCharacterMovement();
}
/* End of Character Getters Interface */

//////////////////////////////////////////////////////////////////////////
// Input
/* Character Controls Interface*/
void ADuskfallCharacter::JumpPressed_Implementation()
{
	if (JumpAudio != nullptr && GetCharacterMovement()->IsMovingOnGround())
	{
		MovementAudioComponent->SetSound(JumpAudio);
		MovementAudioComponent->Play(0.0f);
	}
	Jump();
}

void ADuskfallCharacter::JumpReleased_Implementation()
{
	StopJumping();
}

void ADuskfallCharacter::AttackPressed_Implementation()
{
	if (CurrentStamina > 0)
	{
		if (CharacterState == ECharacterState::ECS_Blocking)
		{
			if (Shield != nullptr)
			{
				Shield->ParryPressed();
			}
		}
		else
		{
			if (Weapon != nullptr && CharacterState != ECharacterState::ECS_Attacking)
			{
				Weapon->AttackPressed();
			}
		}
	}
}

void ADuskfallCharacter::AttackReleased_Implementation()
{
	if (Weapon != nullptr)
	{
		Weapon->AttackReleased();
	}
}

void ADuskfallCharacter::BlockPressed_Implementation()
{
	if (CurrentStamina > 0)
	{
		if (Shield != nullptr)
		{
			Shield->BlockPressed();
		}
	}
}

void ADuskfallCharacter::BlockReleased_Implementation()
{
	if (CurrentStamina > 0)
	{
		if (Shield != nullptr)
		{
			Shield->BlockReleased();
		}
	}
}

void ADuskfallCharacter::DashPressed_Implementation()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		if (CurrentStamina > 0)
		{
			Dash();
		}
	}
}

void ADuskfallCharacter::DashReleased_Implementation()
{
}

void ADuskfallCharacter::UsePressed_Implementation()
{
}

void ADuskfallCharacter::UseReleased_Implementation()
{
}

void ADuskfallCharacter::MoveForward_Implementation(float Scale)
{
}

void ADuskfallCharacter::MoveRight_Implementation(float Scale)
{
}

void ADuskfallCharacter::TurnRate_Implementation(float Scale)
{
}

void ADuskfallCharacter::Turn_Implementation(float Scale)
{
}
/* End of character controls interface */
