// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Components/Equipment/Equipment.h"
#include "AdvBaseShield.generated.h"


/**
 * 
 */

UENUM()
enum EAdvShieldState
{
	ASS_Idle   UMETA(DisplayName = "Idle"),
	ASS_WindingUp     UMETA(DisplayName = "WindingUp"),
	ASS_Parry  UMETA(DisplayName = "Parry"),
	ASS_Blocking UMETA(DisplayName = "Blocking"),
	ASS_WindingDown UMETA(DisplayName = "WindingDown"),
};

UCLASS()
class DUSKFALL_API AAdvBaseShield : public AEquipment
{
	GENERATED_BODY()
	
//values
public:

protected:

#pragma region Animations

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* ParryAnim;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* BlockingAnim;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* LowerShieldAnim;

#pragma endregion Contains all the animations for the melee weapon

#pragma region Standard Melee Weapon Values

	UPROPERTY(EditDefaultsOnly, Category = "Melee Weapon Stats")
		TEnumAsByte<EAdvShieldState> ShieldState = EAdvShieldState::ASS_Idle;

#pragma endregion contains state, damage, Hitbox, and stacked input

private:

	bool HasBeenReleased = false;


//functiosn
public:

	AAdvBaseShield();

	virtual void UsePressed() override;

	virtual void UseReleased() override;

	virtual void Equipped(UEquipmentManager* EquipmentManager, UAnimBillboardComponent* NewAnimBillboard = nullptr) override;

	virtual void AnimEventHit_Implementation(FString& AnimMessage) override;

protected:

private:

	void StartParry();
	void FinishParry();
	void ShieldLowered();

};
