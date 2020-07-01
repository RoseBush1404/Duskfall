// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Components/Equipment/Equipment.h"
#include "AdvBaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DUSKFALL_API AAdvBaseWeapon : public AEquipment
{
	GENERATED_BODY()
	
//values
public:

protected:

private:


//functions
public:

	AAdvBaseWeapon();

	virtual void UsePressed() override;

	virtual void UseReleased() override;

	virtual void Equipped(UEquipmentManager* EquipmentManager, UAnimBillboardComponent* NewAnimBillboard = nullptr) override;

protected:

private:

};
