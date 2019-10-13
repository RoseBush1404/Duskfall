// Duskfall - Michael Rose - r015676g


#include "BasePlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Camera/PlayerCameraManager.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

//////////////////////////////////////////////////////////////////////////
// Input
/* Character Controls Interface*/
void ABasePlayerCharacter::MoveForward_Implementation(float Scale)
{
	CharacterMoveForward(Scale);
}

void ABasePlayerCharacter::MoveRight_Implementation(float Scale)
{
	CharacterMoveRight(Scale);
}

void ABasePlayerCharacter::TurnRate_Implementation(float Scale)
{
	CharacterTurnAtRate(Scale);
}

void ABasePlayerCharacter::Turn_Implementation(float Scale)
{
	AddControllerYawInput(Scale);
}
/* End of character controls interface */

void ABasePlayerCharacter::CharacterMoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		// add camera shake
		if (WalkingCameraShake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WalkingCameraShake, 1.0f);
		}
	}
}

void ABasePlayerCharacter::CharacterMoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		// add camera shake
		if (WalkingCameraShake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WalkingCameraShake, 1.0f);
		}
	}
}



void ABasePlayerCharacter::CharacterTurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}