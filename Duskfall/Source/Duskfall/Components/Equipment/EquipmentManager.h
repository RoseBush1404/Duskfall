// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentManager.generated.h"

class AEquipment;
class UAnimBillboardComponent;

enum EEquipmentType;

UENUM()
enum ESlotState
{
	SS_Enabled   UMETA(DisplayName = "Enabled"),
	SS_Disabled   UMETA(DisplayName = "Disabled")
};

USTRUCT(BlueprintType)
struct FEquipmentSlotType
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly)
		bool Weapon = false;

	UPROPERTY(EditDefaultsOnly)
		bool Shield = false;

	UPROPERTY(EditDefaultsOnly)
		bool Armour = false;

	UPROPERTY(EditDefaultsOnly)
		bool Item = false;

	FEquipmentSlotType(bool WeaponSet = false, bool ShieldSet = false, bool ArmourSet = false, bool ItemSet = false)
	{
		Weapon = WeaponSet;
		Shield = ShieldSet;
		Armour = ArmourSet;
		Item = ItemSet;
	}
};

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY();

	UPROPERTY(VisibleAnywhere)
		FString SlotName = "";

	UPROPERTY(VisibleAnywhere)
		AEquipment* Equipment = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAnimBillboardComponent* AnimationBillBoard = nullptr;

	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<ESlotState> SlotState = ESlotState::SS_Enabled;

	UPROPERTY(VisibleAnywhere)
		FEquipmentSlotType SlotType = FEquipmentSlotType();

	UPROPERTY(VisibleAnywhere)
		FString LinkedSlotName = "";

	FEquipmentSlot(FString NewSlotName = "", AEquipment* NewEquipment = nullptr, UAnimBillboardComponent* NewAnimationBillBoard = nullptr, TEnumAsByte<ESlotState> NewSlotState = ESlotState::SS_Enabled, FEquipmentSlotType NewSlotType = FEquipmentSlotType())
	{
		SlotName = NewSlotName;
		Equipment = NewEquipment;
		AnimationBillBoard = NewAnimationBillBoard;
		SlotState = NewSlotState;
		SlotType = FEquipmentSlotType(NewSlotType);
	}

	void SetLinkedSlotName(FString NewLinkedSlotName)
	{
		LinkedSlotName = NewLinkedSlotName;
	}
};

USTRUCT(BlueprintType)
struct FDefaultEquipment
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly)
		FString SlotName = "";

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AEquipment> DefaultEquipment = nullptr;

	UPROPERTY(EditDefaultsOnly)
		AEquipment* EquipmentPointer = nullptr;

	FDefaultEquipment(FString NewSlotName = "", TSubclassOf<AEquipment> NewDefaultEquipment = nullptr)
	{
		SlotName = NewSlotName;
		DefaultEquipment = NewDefaultEquipment;
		EquipmentPointer = nullptr;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUSKFALL_API UEquipmentManager : public UActorComponent
{
	GENERATED_BODY()

//values
public:

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
		TArray<FEquipmentSlot> EquipmentSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TArray<FDefaultEquipment> DefaultEquipment;

protected:

private:


//functions
public:	
	
	UEquipmentManager();

	void InitEquipmentManager();

#pragma region Equipment Use Functions

	void UsePressedForEquipmentInSlot(FString EquipmentSlotName);

	void UseReleasedForEquipmentInSlot(FString EquipmentSlotName);

	void AltUsePressedForEquipmentInSlot(FString EquipmentSlotName);

	void AltUseReleasedForEquipmentInSlot(FString EquipmentSlotName);

#pragma endregion contains the functions for using equipment

#pragma region Slot Setters

	void SetSlotState(FString SlotName, ESlotState NewSlotState);
	void SetSlotState(AEquipment* Equipment, ESlotState NewSlotState);
	void SetLinkedSlotState(FString SlotName, ESlotState NewSlotState);
	void SetLinkedSlotState(AEquipment* Equipment, ESlotState NewSlotState);

#pragma endregion contains all the setters for changing the slot state

protected:

	void CheckSlotValidation(FString SlotName);

private:	

	void AddDefaultEquipment();

	bool CanEquipmentBeEquipedInSlot(FEquipmentSlotType SlotType, TEnumAsByte<EEquipmentType> EquipmentType);

};
