// Duskfall - Michael Rose - r015676g


#include "BaseWeapon.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/DuskfallCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::AttackPressed()
{
	if (User == nullptr) { return; }

	if (WeaponState == EWeaponState::EWS_NotAttacking)
	{
		//get owner's movement component and set its speed
		UCharacterMovementComponent* MovementComponent = User->GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = AttackMovementSpeed;

		WeaponState = EWeaponState::EWS_WaitingForRelease;

		GetWorld()->GetTimerManager().SetTimer(weaponChargingTimer, this, &ABaseWeapon::AttackingCharging, 0.1f, true);
	}
}

void ABaseWeapon::AttackReleased()
{
	if (User == nullptr) { return; }

	if (WeaponState == EWeaponState::EWS_Charging || WeaponState == EWeaponState::EWS_WaitingForRelease)
	{
		GetWorld()->GetTimerManager().ClearTimer(weaponChargingTimer);
		AttackTriggered();
	}
}

void ABaseWeapon::StopAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(weaponChargingTimer);
	GetWorld()->GetTimerManager().ClearTimer(WindUpDelayTimer);
	GetWorld()->GetTimerManager().ClearTimer(WindDownDelayTimer);
	GetWorld()->GetTimerManager().ClearTimer(AttackFinishDelayTimer);
	chargeAmount = 0.0f;
	UCharacterMovementComponent* MovementComponent = User->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = DefaultMovementSpeed;
	WeaponState = EWeaponState::EWS_NotAttacking;
	if (User->GetCharacterState() == ECharacterState::ECS_Parryed || User->GetCharacterState() == ECharacterState::ECS_Staggered) { return; }
	FlipbookComponent->SetFlipbook(IdleWeaponAnimation);
}

void ABaseWeapon::SetFlipbook(UPaperFlipbookComponent * Flipbook)
{
	FlipbookComponent = Flipbook; 
	FlipbookComponent->SetFlipbook(IdleWeaponAnimation);
}


void ABaseWeapon::AttackTriggered()
{
	SetChargeModifier();

	// tell character to use X amount of stamina
	User->DecreaseStamina(AttackStaminaDrain * ChargeModifier);

	if (ChargeModifier > 1.0f)
	{
		if (FlipbookComponent != nullptr)
		{
			FlipbookComponent->SetFlipbook(ChargeAttackAnimation);
		}
	}
	else
	{
		if (FlipbookComponent != nullptr)
		{
			if (StandardAttackAnimations.Num() > 0)
			{
				FlipbookComponent->SetFlipbook(StandardAttackAnimations[NextPlayedAttackIndex]);
				NextPlayedAttackIndex++;
				if (NextPlayedAttackIndex >= StandardAttackAnimations.Num()) { NextPlayedAttackIndex = 0; }
			}
		}
	}
	WeaponState = EWeaponState::EWS_WindingUp;

	User->PlayGivenCameraShake(AttackCameraShake, ChargeModifier);

	GetWorld()->GetTimerManager().SetTimer(WindUpDelayTimer, this, &ABaseWeapon::HitEffect, AttackWindUp, false);
}

void ABaseWeapon::HitEffect()
{
	//child classes override this to get the weapon to do its damage
	WeaponState = EWeaponState::EWS_Damage;
	GetWorld()->GetTimerManager().ClearTimer(WindUpDelayTimer);
	GetWorld()->GetTimerManager().SetTimer(WindDownDelayTimer, this, &ABaseWeapon::WindDownTrigger, AttackWindDown, false);
}

void ABaseWeapon::WindDownTrigger()
{
	WeaponState = EWeaponState::EWS_WindingDown;
	GetWorld()->GetTimerManager().ClearTimer(WindDownDelayTimer);

	//reset movement speed
	UCharacterMovementComponent* MovementComponent = User->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = DefaultMovementSpeed;
	chargeAmount = 0.0f;
	if (User->GetCharacterState() == ECharacterState::ECS_Parryed || User->GetCharacterState() == ECharacterState::ECS_Staggered)
	{
		WeaponState = EWeaponState::EWS_NotAttacking;
		return;
	}
	User->UpdateCharacterState(ECharacterState::ECS_Moveable);

	GetWorld()->GetTimerManager().SetTimer(AttackFinishDelayTimer, this, &ABaseWeapon::AttackFinished, RemainAttackTime, false);
}

void ABaseWeapon::AttackFinished()
{
	WeaponState = EWeaponState::EWS_NotAttacking;
	GetWorld()->GetTimerManager().ClearTimer(AttackFinishDelayTimer);
	if (User->GetCharacterState() == ECharacterState::ECS_Parryed || User->GetCharacterState() == ECharacterState::ECS_Staggered) { return; }
	FlipbookComponent->SetFlipbook(IdleWeaponAnimation);
}

void ABaseWeapon::AttackingCharging()
{
	chargeAmount = chargeAmount + 0.1f;

	if (chargeAmount >= MinChargeAmount && WeaponState != EWeaponState::EWS_Charging)
	{
		//set state and update flipbook
		WeaponState = EWeaponState::EWS_Charging;
		if (FlipbookComponent != nullptr)
		{
			FlipbookComponent->SetFlipbook(ChargingWeaponAnimation);
		}
	}
	chargeAmount = FMath::Clamp(chargeAmount, 0.0f, MaxChargeAmount);
}

void ABaseWeapon::SetChargeModifier()
{
	if (chargeAmount >= MinChargeAmount)
	{
		if (chargeAmount >= MaxChargeAmount)
		{
			ChargeModifier = 2.0f;
		}
		else
		{
			ChargeModifier = 1.5f;
		}
	}
	else
	{
		ChargeModifier = 1.0f;
	}
}
