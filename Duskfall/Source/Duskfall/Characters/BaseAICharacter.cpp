// Duskfall - Michael Rose - r015676g


#include "BaseAICharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "../Weapons/BaseWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/Vector.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipBook"));
	CharacterFlipbook->SetupAttachment(GetCapsuleComponent());
	CharacterFlipbook->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CharacterFlipbook->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	CharacterFlipbook->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Weapon->SetFlipbook(CharacterFlipbook);

	PlayerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();

	GetWorld()->GetTimerManager().SetTimer(AIFacingTimer, this, &ABaseAICharacter::FacePlayer, AIFacingTickRate, true);
}

void ABaseAICharacter::FacePlayer()
{
	if (PlayerTarget == nullptr) { return; }

	LookAtDirection = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerTarget->GetActorLocation());

	if (!CanChangePitchOfMuzzle)
	{
		SetActorRotation(LookAtDirection);
	}
	if (CanChangePitchOfMuzzle)
	{
		UpdateMuzzleRotation(LookAtDirection);
		CharacterFlipbook->SetWorldRotation(FRotator( 0, LookAtDirection.Yaw + 90, 0));
	}

	if (CharacterState == ECharacterState::ECS_Moveable)
	{
		if (GetVelocity().Equals(FVector(0.0f, 0.0f, 0.0f), 0.1f))
		{
			CharacterFlipbook->SetFlipbook(IdleCharacterAnimation);
		}
		else
		{
			CharacterFlipbook->SetFlipbook(WalkingCharacterAnimation);
		}
	}
}

void ABaseAICharacter::TakeDamage_Implementation(float Damage, float DamageMoifier, AActor * DamageCauser)
{
	if (DamageCauser == this) { return; }
	if (Cast<ABaseAICharacter>(DamageCauser)) { return; }

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
		break;
	case ECharacterState::ECS_Staggered:
		RemoveHealth(Damage * DamageMoifier);
		break;
	case ECharacterState::ECS_Parrying:
		break;
	case ECharacterState::ECS_Parryed:
		RemoveHealth((Damage * DamageMoifier) * 2);
		break;
	case ECharacterState::ECS_UsingItem:
		RemoveHealth(Damage * DamageMoifier);
		break;
	default:
		break;
	}

	if (Weapon != nullptr) { Weapon->StopAttack(); }

	StartOfDamageFlash();
	CharacterFlipbook->SetFlipbook(StaggeredCharacterAnimation);
	GetWorld()->GetTimerManager().ClearTimer(ParryedDelayTimer);

	if (CurrentHealth > 0)
	{
		CharacterState = ECharacterState::ECS_Staggered;
		Knockback(DamageMoifier, DamageCauser->GetActorLocation());
		GetWorld()->GetTimerManager().SetTimer(StaggerDelayTimer, this, &ABaseAICharacter::EndStagger, StaggerLength, false);
	}
	else
	{
		CharacterState = ECharacterState::ECS_Died;

		if (DeathAudio != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
		}
		//delay
		GetWorld()->GetTimerManager().SetTimer(DeathDelayTimer, this, &ABaseAICharacter::Die, DelayBeforeDeath, false);
	}
}

void ABaseAICharacter::StartOfDamageFlash()
{
	CharacterFlipbook->SetSpriteColor(FLinearColor::Red);
	GetWorld()->GetTimerManager().SetTimer(DamageFlashTimer, this, &ABaseAICharacter::EndOfDamageFlash, DamageFlashLength, false);
}

void ABaseAICharacter::EndOfDamageFlash()
{
	GetWorld()->GetTimerManager().ClearTimer(DamageFlashTimer);
	CharacterFlipbook->SetSpriteColor(FLinearColor::White);
}

void ABaseAICharacter::Knockback(float Modifier, FVector SourceLocation)
{
	FVector UnitDirection = GetActorLocation() - SourceLocation;
	FVector KnockbackDirection = (FVector(UnitDirection.X, UnitDirection.Y, 0.0f) * KnockbackBackwardForce);

	GetCharacterMovement()->AddImpulse(FVector(0.0f, 0.0f, KnockbackUpwardForce), true);
	GetCharacterMovement()->AddImpulse(KnockbackDirection * Modifier, true);
}

void ABaseAICharacter::CharacterParryed()
{
	if (Weapon != nullptr)
	{
		Weapon->StopAttack();
	}
	if (ParriedAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ParriedAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}
	CharacterFlipbook->SetFlipbook(StaggeredCharacterAnimation);
	CharacterState = ECharacterState::ECS_Parryed;
	GetWorld()->GetTimerManager().SetTimer(ParryedDelayTimer, this, &ADuskfallCharacter::EndStagger, ParryedLength, false);
}

void ABaseAICharacter::Die()
{
	Destroy();
}
