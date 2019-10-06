// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "Characters/DuskfallCharacter.h"
#include "BasePlayerCharacter.generated.h"

class UInputComponent;

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

protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** looking left and right*/
	void TurnAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShake> WalkingCameraShake;
};
