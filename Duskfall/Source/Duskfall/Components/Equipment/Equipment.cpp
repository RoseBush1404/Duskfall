// Duskfall - Michael Rose - r015676g


#include "Equipment.h"
#include "Components/SpriteAnimConfig.h"
#include "Components/AnimBillboardComponent.h"
#include "Components/Equipment/EquipmentManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/AdvCharacterGetters.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values
AEquipment::AEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipment::UsePressed()
{
}

void AEquipment::UseReleased()
{
}

void AEquipment::AltUsePressed()
{
}

void AEquipment::AltUseReleased()
{
}

void AEquipment::Equipped(UEquipmentManager* EquipmentManager,UAnimBillboardComponent* NewAnimBillboard)
{
	if (NewAnimBillboard != nullptr)
	{
		AnimBillboard = NewAnimBillboard;
		AnimBillboard->SetUser(this);
		AnimBillboard->PlayAnimation(StandardSpriteAnim);
	}

	switch (EquipmentSize)
	{
	case EEquipmentSize::ES_TwoHanded:
		// tell equipment manager that this item is two handed and will need to lock the linked slot
		EquipmentManager->SetLinkedSlotState(this, ESlotState::SS_Disabled);
	break;

	default:
		break;
	}
}

void AEquipment::Unequipped(UEquipmentManager* EquipmentManager)
{
	if (AnimBillboard != nullptr)
	{
		AnimBillboard->SetUser(nullptr);
		AnimBillboard->StopAnimation();
		AnimBillboard = nullptr;
	}

	switch (EquipmentSize)
	{
	case EEquipmentSize::ES_TwoHanded:
		// tell equipment manager that this item is two handed and will need to now unlock the linked slot
		EquipmentManager->SetLinkedSlotState(this, ESlotState::SS_Enabled);
		break;

	default:
		break;
	}
}

void AEquipment::SetUser(AActor* UserRef)
{
	User = UserRef;
}

#pragma region Anim Event Nofifier

void AEquipment::AnimEventHit_Implementation(FString& AnimMessage)
{

}

#pragma endregion

#pragma region User Compoent Getters

void AEquipment::GetUsersMovementComponent()
{
	if (User != nullptr)
	{
		IAdvCharacterGetters* CharacterGetter = Cast<IAdvCharacterGetters>(User);
		if (CharacterGetter != nullptr) //c++ layer
		{
			UsersMovementComponent = CharacterGetter->Execute_GetCharacterMovementComponent(User);
		}
		else if (User->GetClass() && User->GetClass()->ImplementsInterface(UAdvCharacterGetters::StaticClass())) //blueprint layer
		{
			UsersMovementComponent = IAdvCharacterGetters::Execute_GetCharacterMovementComponent(User);
		}
	}
}

void AEquipment::GetUserCameraManager()
{
	if (User != nullptr)
	{
		IAdvCharacterGetters* CharacterGetter = Cast<IAdvCharacterGetters>(User);
		if (CharacterGetter != nullptr) //c++ layer
		{
			UsersCameraManager = CharacterGetter->Execute_GetPlayerCameraManager(User);
		}
		else if (User->GetClass() && User->GetClass()->ImplementsInterface(UAdvCharacterGetters::StaticClass())) //blueprint layer
		{
			UsersCameraManager = IAdvCharacterGetters::Execute_GetPlayerCameraManager(User);
		}
	}
}
#pragma endregion
