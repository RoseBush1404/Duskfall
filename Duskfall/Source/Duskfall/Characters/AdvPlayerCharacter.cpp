// Duskfall - Michael Rose - r015676g


#include "AdvPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AnimBillboardComponent.h"
#include "Components/Equipment/Equipment.h"
#include "Components/Equipment/EquipmentManager.h"

AAdvPlayerCharacter::AAdvPlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//create left hand
	LeftHandAnimComponent = CreateDefaultSubobject<UAnimBillboardComponent>(TEXT("LeftHand"));
	LeftHandAnimComponent->SetupAttachment(GetCapsuleComponent());
	LeftHandAnimComponent->SetRelativeLocation(FVector(0.0f, 1.5f, 65.0f));

	//create right hand
	RightHandAnimComponent = CreateDefaultSubobject<UAnimBillboardComponent>(TEXT("RightHand"));
	RightHandAnimComponent->SetupAttachment(GetCapsuleComponent());
	RightHandAnimComponent->SetRelativeLocation(FVector(0.1f, 2.5f, 65.0f));

	//Set up equipment slots
	EquipmentManager->EquipmentSlots.Add(FEquipmentSlot("Right Hand", nullptr, RightHandAnimComponent, ESlotState::SS_Enabled, FEquipmentSlotType(true, true, false, true)));
	EquipmentManager->EquipmentSlots.Add(FEquipmentSlot("Left Hand", nullptr, LeftHandAnimComponent, ESlotState::SS_Enabled, FEquipmentSlotType(true, true, false, true)));
	EquipmentManager->DefaultEquipment.Add(FDefaultEquipment("Right Hand", nullptr));
	EquipmentManager->DefaultEquipment.Add(FDefaultEquipment("Left Hand", nullptr));
}

void AAdvPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//set up links for slots
	EquipmentManager->EquipmentSlots[0].SetLinkedSlotName("Left Hand");
	EquipmentManager->EquipmentSlots[1].SetLinkedSlotName("Right Hand");

	EquipmentManager->InitEquipmentManager();
}

#pragma region Character Controls overrides

void AAdvPlayerCharacter::AttackPressed_Implementation()
{
	EquipmentManager->UsePressedForEquipmentInSlot("Right Hand");
}

void AAdvPlayerCharacter::AttackReleased_Implementation()
{
	EquipmentManager->UseReleasedForEquipmentInSlot("Right Hand");
}

void AAdvPlayerCharacter::BlockPressed_Implementation()
{
	EquipmentManager->UsePressedForEquipmentInSlot("Left Hand");
}

void AAdvPlayerCharacter::BlockReleased_Implementation()
{
	EquipmentManager->UseReleasedForEquipmentInSlot("Left Hand");
}

void AAdvPlayerCharacter::MoveForward_Implementation(float Scale)
{
	Super::MoveForward_Implementation(Scale);

	if (Scale != 0.0f)
	{
		// add camera shake
		if (WalkingCameraShake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WalkingCameraShake, 1.0f);
		}
	}
}

void AAdvPlayerCharacter::MoveRight_Implementation(float Scale)
{
	Super::MoveRight_Implementation(Scale);

	if (Scale != 0.0f)
	{
		// add camera shake
		if (WalkingCameraShake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WalkingCameraShake, 1.0f);
		}
	}
}

#pragma endregion

#pragma region Character Getter overrides

APlayerCameraManager* AAdvPlayerCharacter::GetPlayerCameraManager_Implementation()
{
	return GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

#pragma endregion
