// Duskfall - Michael Rose - r015676g


#include "AdvMeleeWeapon.h"
#include "Components/AnimBillboardComponent.h"
#include "Characters/HealthSystem.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShake.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

#pragma region Input

void AAdvMeleeWeapon::UsePressed()
{
	if (AnimBillboard != nullptr)
	{
		if (WeaponState == EAdvWeaponState::AWS_Idle || WeaponState == EAdvWeaponState::AWS_Reset)
		{
			GetWorld()->GetTimerManager().SetTimer(ChargeAttackTimer, this, &AAdvMeleeWeapon::AttackCharging, 0.2f, false);
		}
	}
}

void AAdvMeleeWeapon::UseReleased()
{
	if (AnimBillboard != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargeAttackTimer);

		if (WeaponState == EAdvWeaponState::AWS_Idle || WeaponState == EAdvWeaponState::AWS_Reset)
		{
			WeaponState = EAdvWeaponState::AWS_WindingUp;
			UsersMovementComponent->MaxWalkSpeed = UsersMovementComponent->MaxWalkSpeed - MovementSpeedModifer;
			TriggerAttackEffects();
			AnimBillboard->PlayAnimation(AttackAnim[0]);
		}
		else if (WeaponState == EAdvWeaponState::AWS_WindingDown)
		{
			HasStackedInput = true;
		}
		else if (WeaponState == EAdvWeaponState::AWS_Charging)
		{
			DoChargeAttack();
		}
	}
}

#pragma endregion contains the input fucntions

#pragma region Charge Attack Functions

void AAdvMeleeWeapon::AttackCharging()
{
	WeaponState = EAdvWeaponState::AWS_Charging;
	UsersMovementComponent->MaxWalkSpeed = UsersMovementComponent->MaxWalkSpeed - MovementSpeedModifer;
	AttackModifer = 1.25f;
	AnimBillboard->PlayAnimation(AttackChargingAnim);
}

void AAdvMeleeWeapon::DoChargeAttack()
{
	WeaponState = EAdvWeaponState::AWS_WindingUp;
	TriggerAttackEffects();
	AnimBillboard->PlayAnimation(ChargeAttackAnim);
}

#pragma endregion conntains the functions for the melee charge attack

#pragma region Anim Events

void AAdvMeleeWeapon::AnimEventHit_Implementation(FString& AnimMessage)
{
	if (AnimMessage == "HitEvent")
	{
		HitEvent();
	}

	if (AnimMessage == "WindingDownEvent")
	{
		WindingDownEvent();
	}

	if (AnimMessage == "AttackFinishedEvent")
	{
		AttackFinishedEvent();
	}

	if (AnimMessage == "AttackResetFinishedEvent")
	{
		AttackResetFinishedEvent();
	}

	if (AnimMessage == "AttackHalfCharged")
	{
		AttackHalfCharged();
	}

	if (AnimMessage == "AttackFullyCharged")
	{
		AttackFullyCharged();
	}
}

void AAdvMeleeWeapon::HitEvent()
{
	WeaponState = EAdvWeaponState::AWS_Damage;
	HitScan();
}

void AAdvMeleeWeapon::WindingDownEvent()
{
	WeaponState = EAdvWeaponState::AWS_WindingDown;
}

void AAdvMeleeWeapon::AttackFinishedEvent()
{
	AttackModifer = 1.0f;

	if (HasStackedInput)
	{
		PlayNextAttackAnim();
	}
	else
	{
		WeaponState = EAdvWeaponState::AWS_Reset;
		UsersMovementComponent->MaxWalkSpeed = UsersMovementComponent->MaxWalkSpeed + MovementSpeedModifer;
		AnimBillboard->PlayAnimation(AttackResetAnim);
	}
}

void AAdvMeleeWeapon::AttackResetFinishedEvent()
{
	WeaponState = EAdvWeaponState::AWS_Idle;
	AnimBillboard->PlayAnimation(StandardSpriteAnim);
}

void AAdvMeleeWeapon::AttackHalfCharged()
{
	AttackModifer = 1.5f;
}

void AAdvMeleeWeapon::AttackFullyCharged()
{
	AttackModifer = 2.0f;
}

void AAdvMeleeWeapon::HitScan()
{
	if (User != nullptr)
	{
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(HitBoxHalfSize);
		FVector StartLocation = User->GetActorLocation() + (FVector(0.0f, 0.0f, HitBoxOffSet.Z)) + (User->GetActorForwardVector() * HitBoxOffSet.X);
		FVector EndLocation = User->GetActorLocation() + (FVector(0.0f, 0.0f, HitBoxOffSet.Z)) + (User->GetActorForwardVector() * HitBoxOffSet.X);
		FRotator Rotator = User->GetActorRotation();
		FQuat ForwardRotation = Rotator.Quaternion();

		TArray<FHitResult> OutResults;
		bool isHit = GetWorld()->SweepMultiByChannel(OutResults, StartLocation, EndLocation, ForwardRotation, ECollisionChannel::ECC_Visibility, CollisionShape);
		//DrawDebugBox(GetWorld(), StartLocation, HitBoxHalfSize, ForwardRotation, FColor::Red, false, 5.0f);

		if (isHit)
		{
			for (auto& Hit : OutResults)
			{
				if (Hit.GetActor() != User)
				{
					IHealthSystem* HealthSystem = Cast<IHealthSystem>(Hit.GetActor());
					if (HealthSystem != nullptr) // C++ Layer
					{
						HealthSystem->Execute_TakeDamage(Hit.GetActor(), BaseDamage, AttackModifer, User);
					}
					else if (Hit.GetActor()->GetClass() != nullptr && Hit.GetActor()->GetClass()->ImplementsInterface(UHealthSystem::StaticClass())) // Blueprint Layer
					{
						IHealthSystem::Execute_TakeDamage(Hit.GetActor(), BaseDamage, AttackModifer, User);
					}
				}
			}
		}
	}
}
#pragma endregion contains the anim event hit interface and all the melee weapons events

void AAdvMeleeWeapon::PlayNextAttackAnim()
{
	HasStackedInput = false;
	CurrentComboCount++;
	if (CurrentComboCount + 1 > AttackAnim.Num()) { CurrentComboCount = 0; }

	WeaponState = EAdvWeaponState::AWS_WindingUp;
	TriggerAttackEffects();
	AnimBillboard->PlayAnimation(AttackAnim[CurrentComboCount]);
}

void AAdvMeleeWeapon::TriggerAttackEffects()
{
	if (UsersCameraManager != nullptr) 
	{ 
		UsersCameraManager->PlayCameraShake(AttackCameraShake, AttackModifer); 
	}

	if (AttackAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}
}
