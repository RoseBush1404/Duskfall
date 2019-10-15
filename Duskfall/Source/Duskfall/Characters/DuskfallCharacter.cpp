// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "../Weapons/BaseWeapon.h"
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

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(StartingWeapon, GetActorTransform());
	Weapon->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	Weapon->SetUser(this);
	Weapon->SetMuzzlePoint(MuzzlePoint);
	UCharacterMovementComponent* MovementComponent = this->GetCharacterMovement();
	Weapon->SetDefaultMovementSpeed(MovementComponent->MaxWalkSpeed);

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
	CharacterState = ECharacterState::ECS_Attacking;
	Weapon->AttackPressed();
}

void ADuskfallCharacter::AttackReleased_Implementation()
{
	Weapon->AttackReleased();
}

void ADuskfallCharacter::BlockPressed_Implementation()
{
}

void ADuskfallCharacter::BlockReleased_Implementation()
{
}

void ADuskfallCharacter::DashPressed_Implementation()
{
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
