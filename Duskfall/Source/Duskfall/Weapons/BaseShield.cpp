// Duskfall - Michael Rose - r015676g


#include "BaseShield.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "../Characters/DuskfallCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ABaseShield::ABaseShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseShield::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseShield::SetFlipbook(UPaperFlipbookComponent * Flipbook)
{
	FlipbookComponent = Flipbook;
	FlipbookComponent->SetFlipbook(IdleShieldAnimation);
}

void ABaseShield::BlockPressed()
{
	if (User == nullptr) { return; }

	User->UpdateCharacterState(ECharacterState::ECS_Blocking);
	if (FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetFlipbook(BlockingShieldAnimation);
	}
	UCharacterMovementComponent* MovementComponent = User->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = BlockingMovementSpeed;
}

void ABaseShield::BlockReleased()
{
	if (User == nullptr) { return; }

	User->UpdateCharacterState(ECharacterState::ECS_Moveable);
	if (FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetFlipbook(IdleShieldAnimation);
	}
	UCharacterMovementComponent* MovementComponent = User->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = DefaultMovementSpeed;
}

void ABaseShield::ParryPressed()
{
	if (User == nullptr) { return; }

	User->UpdateCharacterState(ECharacterState::ECS_Parrying);
	User->DecreaseStamina(ParryStaminaDrain);
	if (FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetFlipbook(ParryShieldAnimation);
	}
	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, this, &ABaseShield::ParryFinished, ParryTime, false);
}

void ABaseShield::SuccessfulBlock()
{
	if (BlockAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}
}

void ABaseShield::SuccessfultParry()
{
	if (ParryAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ParryAudio, GetActorLocation(), MasterVolume, 1.0f, 0.0f);
	}
}

void ABaseShield::ParryFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(ParryTimerHandle);
	DropShield();
}

void ABaseShield::DropShield()
{
	BlockReleased();
}