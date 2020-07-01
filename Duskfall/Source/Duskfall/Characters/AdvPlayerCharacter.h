// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Characters/AdvDuskfallCharacter.h"
#include "AdvPlayerCharacter.generated.h"

/**
 * 
 */

class UAnimBillboardComponent;

UCLASS()
class DUSKFALL_API AAdvPlayerCharacter : public AAdvDuskfallCharacter
{
	GENERATED_BODY()

//value
public:
	
	AAdvPlayerCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShake> WalkingCameraShake;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
		UAnimBillboardComponent* LeftHandAnimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
		UAnimBillboardComponent* RightHandAnimComponent;

private:


//functions
public:

#pragma region Character Control overrides

	virtual void AttackPressed_Implementation() override;
	virtual void AttackReleased_Implementation() override;
	virtual void BlockPressed_Implementation() override;
	virtual void BlockReleased_Implementation() override;
	virtual void MoveForward_Implementation(float Scale) override;
	virtual void MoveRight_Implementation(float Scale) override;

#pragma endregion

#pragma region Character Getter overrides

	virtual APlayerCameraManager* GetPlayerCameraManager_Implementation() override;

#pragma endregion


protected:

	virtual void BeginPlay() override;

private:

};
