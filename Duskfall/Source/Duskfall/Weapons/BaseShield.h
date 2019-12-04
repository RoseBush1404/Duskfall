// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "BaseShield.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class ADuskfallCharacter;
class USoundCue;

UCLASS()
class DUSKFALL_API ABaseShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShield();

	void BlockPressed();

	void BlockReleased();

	void ParryPressed();

	void SuccessfulBlock();

	void SuccessfultParry();

	void SetUser(ADuskfallCharacter* Character) { User = Character; }

	void SetFlipbook(UPaperFlipbookComponent* Flipbook);

	void SetDefaultMovementSpeed(float MovementSpeed) { DefaultMovementSpeed = MovementSpeed; }

	void DropShield();

	float GetShieldDotProductRange() { return ShieldDotProductRange; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ParryFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		UPaperFlipbook* IdleShieldAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		UPaperFlipbook* BlockingShieldAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		UPaperFlipbook* ShieldHitAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		UPaperFlipbook* ParryShieldAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		float BlockingMovementSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		float ParryStaminaDrain = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		float ParryTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		float ShieldDotProductRange = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Stats")
		TSubclassOf<UCameraShake> ShieldHitCameraShake;

	ADuskfallCharacter* User;

	UPaperFlipbookComponent* FlipbookComponent;

	float DefaultMovementSpeed;

	FTimerHandle ParryTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* BlockAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ParryAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;
};
