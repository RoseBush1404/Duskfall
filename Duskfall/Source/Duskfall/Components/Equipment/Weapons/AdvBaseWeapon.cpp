// Duskfall - Michael Rose - r015676g


#include "AdvBaseWeapon.h"

AAdvBaseWeapon::AAdvBaseWeapon()
{
	//Set Equipment Type
	EquipmentType = EEquipmentType::ET_Weapon;
	EquipmentSize = EEquipmentSize::ES_OneHanded;
}

void AAdvBaseWeapon::UsePressed()
{
}

void AAdvBaseWeapon::UseReleased()
{
}

void AAdvBaseWeapon::Equipped(UEquipmentManager* EquipmentManager, UAnimBillboardComponent* NewAnimBillboard)
{
	Super::Equipped(EquipmentManager, NewAnimBillboard);

	GetUsersMovementComponent();
	GetUserCameraManager();
}
