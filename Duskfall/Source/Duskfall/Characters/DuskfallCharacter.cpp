// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ADuskfallCharacter

ADuskfallCharacter::ADuskfallCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
}

void ADuskfallCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

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
}

void ADuskfallCharacter::AttackReleased_Implementation()
{
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
