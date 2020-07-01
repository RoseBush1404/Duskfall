// Duskfall - Michael Rose - r015676g


#include "AdvBaseShield.h"
#include "Components/AnimBillboardComponent.h"

AAdvBaseShield::AAdvBaseShield()
{
	//Set Equipment Type
	EquipmentType = EEquipmentType::ET_Shield;
	EquipmentSize = EEquipmentSize::ES_OneHanded;
}

void AAdvBaseShield::Equipped(UEquipmentManager* EquipmentManager, UAnimBillboardComponent* NewAnimBillboard)
{
	Super::Equipped(EquipmentManager, NewAnimBillboard);

	GetUsersMovementComponent();
	GetUserCameraManager();
}

void AAdvBaseShield::UsePressed()
{
	if (AnimBillboard != nullptr)
	{
		if (ShieldState == EAdvShieldState::ASS_Idle || ShieldState == EAdvShieldState::ASS_WindingDown)
		{
			HasBeenReleased = false;
			ShieldState = EAdvShieldState::ASS_WindingUp;
			AnimBillboard->PlayAnimation(ParryAnim);
		}
	}
}

void AAdvBaseShield::UseReleased()
{
	if (AnimBillboard != nullptr)
	{
		if (ShieldState == EAdvShieldState::ASS_Blocking)
		{
			ShieldState = EAdvShieldState::ASS_WindingDown;
			AnimBillboard->PlayAnimation(LowerShieldAnim);
		}
		else if (ShieldState == EAdvShieldState::ASS_WindingUp || ShieldState == EAdvShieldState::ASS_Parry)
		{
			HasBeenReleased = true;
		}
	}
}

#pragma region Anim Events

void AAdvBaseShield::AnimEventHit_Implementation(FString& AnimMessage)
{
	if (AnimMessage == "ParryStart")
	{
		StartParry();
	}

	if (AnimMessage == "ParryFinished")
	{
		FinishParry();
	}

	if (AnimMessage == "ShieldLowered")
	{
		ShieldLowered();
	}
}

void AAdvBaseShield::StartParry()
{
	ShieldState = EAdvShieldState::ASS_Parry;
}

void AAdvBaseShield::FinishParry()
{
	// has the block button already bee released?
	if (HasBeenReleased)
	{
		//yes then lower shield
		HasBeenReleased = false;
		ShieldState = EAdvShieldState::ASS_WindingDown;
		AnimBillboard->PlayAnimation(LowerShieldAnim);
	}
	else
	{
		//no then go to blocking
		ShieldState = EAdvShieldState::ASS_Blocking;
		if (AnimBillboard != nullptr)
		{
			AnimBillboard->PlayAnimation(BlockingAnim);
		}
	}
}

void AAdvBaseShield::ShieldLowered()
{
	ShieldState = EAdvShieldState::ASS_Idle;
	if (AnimBillboard != nullptr)
	{
		AnimBillboard->PlayAnimation(StandardSpriteAnim);
	}
}

#pragma endregion
