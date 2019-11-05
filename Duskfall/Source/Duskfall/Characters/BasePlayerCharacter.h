// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Characters/DuskfallCharacter.h"
#include "BasePlayerCharacter.generated.h"

class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class DUSKFALL_API ABasePlayerCharacter : public ADuskfallCharacter
{
	GENERATED_BODY()
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flipbook, meta = (AllowPrivateAccess = "true"))
		UPaperFlipbookComponent* WeaponFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flipbook, meta = (AllowPrivateAccess = "true"))
		UPaperFlipbookComponent* ShieldFlipbook;

public:

	ABasePlayerCharacter();

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual void PlayGivenCameraShake(TSubclassOf<UCameraShake> GivenCameraShake, float Scale) override;

	virtual void TakeDamage_Implementation(float Damage, float DamageMoifier, AActor* DamageCauser) override;

	virtual void MoveForward_Implementation(float Scale) override;
	virtual void MoveRight_Implementation(float Scale) override;
	virtual void TurnRate_Implementation(float Scale) override;
	virtual void Turn_Implementation(float Scale) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void Die();
		void Die_Implementation();

protected:

	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void CharacterMoveForward(float Val);

	/** Handles stafing movement, left and right */
	void CharacterMoveRight(float Val);

	/** looking left and right*/
	void CharacterTurnAtRate(float Rate);

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShake> WalkingCameraShake;
};
