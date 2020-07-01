// Duskfall - Michael Rose - r015676g


#include "EquipmentManager.h"
#include "Components/Equipment/Equipment.h"
#include "Components/AnimBillboardComponent.h"

// Sets default values for this component's properties
UEquipmentManager::UEquipmentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEquipmentManager::InitEquipmentManager()
{
	AddDefaultEquipment();
}

void UEquipmentManager::CheckSlotValidation(FString SlotName)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == SlotName)
		{
			if (EquipmentSlots[i].Equipment != nullptr)
			{
				if (!CanEquipmentBeEquipedInSlot(EquipmentSlots[i].SlotType, EquipmentSlots[i].Equipment->GetEquipmentType()))
				{
					//eject equipment
					EquipmentSlots[i].Equipment->Unequipped(this);
					EquipmentSlots[i].Equipment = nullptr;
				}
				else if (EquipmentSlots[i].SlotState == ESlotState::SS_Disabled)
				{
					//eject equipment
					EquipmentSlots[i].Equipment->Unequipped(this);
					EquipmentSlots[i].Equipment = nullptr;
				}
			}
		}
	}
}

void UEquipmentManager::AddDefaultEquipment()
{
	// add the default equipment
	for (int i = 0; i < DefaultEquipment.Num(); i++)
	{
		if (DefaultEquipment[i].DefaultEquipment != nullptr)
		{
			//spawn the default equipment into the world
			DefaultEquipment[i].EquipmentPointer = GetWorld()->SpawnActor<AEquipment>(DefaultEquipment[i].DefaultEquipment, GetOwner()->GetActorTransform());
			DefaultEquipment[i].EquipmentPointer->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
			DefaultEquipment[i].EquipmentPointer->SetUser(GetOwner());

			//loop though each equipment slot
			for (int e = 0; e < EquipmentSlots.Num(); e++)
			{
				// does the slot names match and if so is the slot empty
				if (EquipmentSlots[e].SlotName == DefaultEquipment[i].SlotName && EquipmentSlots[e].Equipment == nullptr)
				{
					//check the equipment can be equiped in this slot
					if (CanEquipmentBeEquipedInSlot(EquipmentSlots[e].SlotType, DefaultEquipment[i].EquipmentPointer->GetEquipmentType()) && EquipmentSlots[e].SlotState == ESlotState::SS_Enabled)
					{
						//equip the equipment in the slot
						EquipmentSlots[e].Equipment = DefaultEquipment[i].EquipmentPointer;
						EquipmentSlots[e].Equipment->Equipped(this, EquipmentSlots[e].AnimationBillBoard);
					}
				}
			}
		}
	}
}

bool UEquipmentManager::CanEquipmentBeEquipedInSlot(FEquipmentSlotType SlotType, TEnumAsByte<EEquipmentType> EquipmentType)
{
	switch (EquipmentType)
	{
	case EEquipmentType::ET_Weapon:
		if (SlotType.Weapon) { return true; }
		else { return false; }
	break;

	case EEquipmentType::ET_Shield:
		if (SlotType.Shield) { return true; }
		else { return false; }
	break;

	case EEquipmentType::ET_Armour:
		if (SlotType.Armour) { return true; }
		else { return false; }
	break;

	case EEquipmentType::ET_Item:
		if (SlotType.Item) { return true; }
		else { return false; }
	break;

	default:
		return false;
	break;
	}

}

#pragma region Equipment Use Functions

void UEquipmentManager::UsePressedForEquipmentInSlot(FString EquipmentSlotName)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == EquipmentSlotName)
		{
			if (EquipmentSlots[i].Equipment != nullptr)
			{
				if (EquipmentSlots[i].SlotState == ESlotState::SS_Enabled)
				{
					EquipmentSlots[i].Equipment->UsePressed();
					break;
				}
				else if (EquipmentSlots[i].SlotState == ESlotState::SS_Disabled)
				{
					if (EquipmentSlots[i].LinkedSlotName != "")
					{
						AltUsePressedForEquipmentInSlot(EquipmentSlots[i].LinkedSlotName);
						break;
					}
				}
			}
		}
	}
}

void UEquipmentManager::UseReleasedForEquipmentInSlot(FString EquipmentSlotName)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == EquipmentSlotName)
		{
			if (EquipmentSlots[i].Equipment != nullptr)
			{
				if (EquipmentSlots[i].SlotState == ESlotState::SS_Enabled)
				{
					EquipmentSlots[i].Equipment->UseReleased();
					break;
				}
				else if (EquipmentSlots[i].SlotState == ESlotState::SS_Disabled)
				{
					if (EquipmentSlots[i].LinkedSlotName != "")
					{
						AltUseReleasedForEquipmentInSlot(EquipmentSlots[i].LinkedSlotName);
						break;
					}
				}
			}
		}
	}
}

void UEquipmentManager::AltUsePressedForEquipmentInSlot(FString EquipmentSlotName)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == EquipmentSlotName)
		{
			if (EquipmentSlots[i].Equipment != nullptr && EquipmentSlots[i].SlotState == ESlotState::SS_Enabled)
			{
				EquipmentSlots[i].Equipment->AltUsePressed();
				break;
			}
		}
	}
}

void UEquipmentManager::AltUseReleasedForEquipmentInSlot(FString EquipmentSlotName)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == EquipmentSlotName)
		{
			if (EquipmentSlots[i].Equipment != nullptr && EquipmentSlots[i].SlotState == ESlotState::SS_Enabled)
			{
				EquipmentSlots[i].Equipment->AltUseReleased();
				break;
			}
		}
	}
}

#pragma endregion contains the functions for using equipment

#pragma region Slot Setters

void UEquipmentManager::SetSlotState(FString SlotName, ESlotState NewSlotState)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == SlotName)
		{
			EquipmentSlots[i].SlotState = NewSlotState;
			CheckSlotValidation(EquipmentSlots[i].SlotName);
			break;
		}
	}
}

void UEquipmentManager::SetSlotState(AEquipment* Equipment, ESlotState NewSlotState)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].Equipment == Equipment)
		{
			EquipmentSlots[i].SlotState = NewSlotState;
			CheckSlotValidation(EquipmentSlots[i].SlotName);
			break;
		}
	}
}

void UEquipmentManager::SetLinkedSlotState(FString SlotName, ESlotState NewSlotState)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].SlotName == SlotName)
		{
			SetSlotState(EquipmentSlots[i].LinkedSlotName, NewSlotState);
			break;
		}
	}
}

void UEquipmentManager::SetLinkedSlotState(AEquipment* Equipment, ESlotState NewSlotState)
{
	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].Equipment == Equipment)
		{
			SetSlotState(EquipmentSlots[i].LinkedSlotName, NewSlotState);
			break;
		}
	}
}

#pragma endregion contains all the implmentions of the slot state setters
