// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "Characters/DuskfallCharacter.h"
#include "BasePlayerCharacter.generated.h"

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

public:

	ABasePlayerCharacter();

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual void MoveForward_Implementation(float Scale) override;
	virtual void MoveRight_Implementation(float Scale) override;
	virtual void TurnRate_Implementation(float Scale) override;
	virtual void Turn_Implementation(float Scale) override;

protected:

	/** Handles moving forward/backward */
	void CharacterMoveForward(float Val);

	/** Handles stafing movement, left and right */
	void CharacterMoveRight(float Val);

	/** looking left and right*/
	void CharacterTurnAtRate(float Rate);

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShake> WalkingCameraShake;
};
