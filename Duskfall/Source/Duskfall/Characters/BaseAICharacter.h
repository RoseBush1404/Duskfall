// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Characters/DuskfallCharacter.h"
#include "BaseAICharacter.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class DUSKFALL_API ABaseAICharacter : public ADuskfallCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flipbook, meta = (AllowPrivateAccess = "true"))
		UPaperFlipbookComponent* CharacterFlipbook;

public:

	ABaseAICharacter();

	virtual void TakeDamage_Implementation(float Damage, float DamageMoifier, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void FacePlayer();

	void StartOfDamageFlash();
	void EndOfDamageFlash();
	void Knockback(float Modifier, FVector SourceLocation);
	virtual void CharacterParryed() override;
	virtual void Die() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		UPaperFlipbook* IdleCharacterAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		UPaperFlipbook* WalkingCharacterAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		UPaperFlipbook* StaggeredCharacterAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		float AIFacingTickRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		float DamageFlashLength = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		float KnockbackUpwardForce = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		float KnockbackBackwardForce = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Stats")
		float DelayBeforeDeath = 1.0f;

	APawn* PlayerTarget;
	FRotator LookAtDirection;

	FTimerHandle AIFacingTimer;
	FTimerHandle DamageFlashTimer;
	FTimerHandle DeathDelayTimer;
};
