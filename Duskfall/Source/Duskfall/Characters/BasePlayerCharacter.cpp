// Duskfall - Michael Rose - r015676g


#include "BasePlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "../Weapons/BaseWeapon.h"
#include "TimerManager.h"


ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	WeaponFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WeaponFlipBook"));
	WeaponFlipbook->SetupAttachment(GetCapsuleComponent());
	WeaponFlipbook->RelativeLocation = FVector(10.0f, 10.0f, 60.0f);
	WeaponFlipbook->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
	
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Weapon->SetFlipbook(WeaponFlipbook);

	GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimer, this, &ADuskfallCharacter::RegenStamina, StaminaRegenRate, true);
}

void ABasePlayerCharacter::PlayGivenCameraShake(TSubclassOf<UCameraShake> GivenCameraShake, float Scale)
{
	if(GivenCameraShake != nullptr)
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(GivenCameraShake, Scale);
}

void ABasePlayerCharacter::TakeDamage_Implementation(float Damage, float DamageMoifier, AActor * DamageCauser)
{
	if (DamageCauser == this) { return; }

	switch (CharacterState)
	{
	case ECharacterState::ECS_Moveable:
		RemoveHealth(Damage * DamageMoifier);
		break;
	case ECharacterState::ECS_Died:
		break;
	case ECharacterState::ECS_Attacking:
		RemoveHealth(Damage * DamageMoifier);
		break;
	case ECharacterState::ECS_Blocking:
		//check to see if they are in front and so block damage
		break;
	case ECharacterState::ECS_Staggered:
		RemoveHealth(Damage * DamageMoifier);
		break;
	case ECharacterState::ECS_Parrying:
		//check if they are in front of you and parry them
		break;
	case ECharacterState::ECS_UsingItem:
		RemoveHealth(Damage * DamageMoifier);
		break;
	default:
		break;
	}
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