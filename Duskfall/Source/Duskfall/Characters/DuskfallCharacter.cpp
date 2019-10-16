// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
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

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(GetCapsuleComponent());
	MuzzlePoint->RelativeLocation = FVector(110.0f, 0.0f, 35.0f);

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
		Weapon->SetMuzzlePoint(MuzzlePoint);
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

void ADuskfallCharacter::RemoveHealth(float Damage)
{
	CurrentHealth = CurrentHealth - Damage;
	if (CurrentHealth <= 0) { Die(); }
}

void ADuskfallCharacter::Die()
{
}

void ADuskfallCharacter::Dash()
{
	//remove x amount of stamina
	CurrentStamina = CurrentStamina - DashStaminaDrain;

	//TODO add shielf element to dash
	//if have shield and using it to block then auto-lower shield

	//add impule
	GetCharacterMovement()->AddImpulse(FVector(0.0f, 0.0f, DashUpwardForce), true);
	GetCharacterMovement()->AddImpulse(GetVelocity() * DashVelocityModifier, true);
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
/* End of Health System Interface */

//////////////////////////////////////////////////////////////////////////
// Input
/* Character Controls Interface*/
void ADuskfallCharacter::JumpPressed_Implementation()
{
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
			if (Weapon != nullptr)
			{
				CharacterState = ECharacterState::ECS_Attacking;
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
	if (Shield != nullptr)
	{
		Shield->BlockPressed();
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
		Dash();
	}
}

void ADuskfallCharacter::DashReleased_Implementation()
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
