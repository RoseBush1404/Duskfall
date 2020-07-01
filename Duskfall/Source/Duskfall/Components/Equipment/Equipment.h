// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AnimEventNotifier.h"
#include "Equipment.generated.h"

class USpriteAnimConfig;
class UAnimBillboardComponent;
class UEquipmentManager;
class UCharacterMovementComponent;

UENUM()
enum EEquipmentType
{
	ET_Weapon    UMETA(DisplayName = "Weapon"),
	ET_Shield    UMETA(DisplayName = "Shield"),
	ET_Armour    UMETA(DisplayName = "Armour"),
	ET_Item   UMETA(DisplayName = "Item")
};

UENUM()
enum EEquipmentSize
{
	ES_OneHanded    UMETA(DisplayName = "One Handed"),
	ES_TwoHanded    UMETA(DisplayName = "Two Handed"),
	ES_Wearable   UMETA(DisplayName = "Wearable"),
};

UCLASS()
class DUSKFALL_API AEquipment : public AActor, public IAnimEventNotifier
{
	GENERATED_BODY()

//values
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Base Equipment Info")
		TEnumAsByte<EEquipmentType> EquipmentType;

	UPROPERTY(EditDefaultsOnly, Category = "Base Equipment Info")
		TEnumAsByte<EEquipmentSize> EquipmentSize;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sprites")
		USpriteAnimConfig* StandardSpriteAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Sprites")
		UAnimBillboardComponent* AnimBillboard = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Movement Stats")
		float MovementSpeedModifer = 200.0f;

	UCharacterMovementComponent* UsersMovementComponent;

	APlayerCameraManager* UsersCameraManager;

	AActor* User;

private:


//functions
public:	
	
	AEquipment();

	virtual void UsePressed();

	virtual void UseReleased();

	virtual void AltUsePressed();

	virtual void AltUseReleased();

	virtual void Equipped(UEquipmentManager* EquipmentManager,UAnimBillboardComponent* NewAnimBillboard = nullptr);
	virtual void Unequipped(UEquipmentManager* EquipmentManager);

	void SetUser(AActor* UserRef);

	TEnumAsByte<EEquipmentType> GetEquipmentType() { return EquipmentType; }

#pragma region Anim Event Notifier Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim Event Notifier")
		void AnimEventHit(FString& AnimMessage);
	virtual void AnimEventHit_Implementation(FString& AnimMessage) override;

#pragma endregion 



protected:
	
	virtual void BeginPlay() override;

	void GetUsersMovementComponent();

	void GetUserCameraManager();

private:	

};
