// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "../Weapons/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/Interface.h"
#include "CharacterGetters.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCharacterGetters : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSKFALL_API ICharacterGetters
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		//void GetAllCharacterStates(TEnumAsByte<ECharacterState>& CharacterStateRef);

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		//ECharacterState GetCharacterState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterGetters")
		ABaseWeapon* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterGetters")
		UCharacterMovementComponent* GetCharacterMovementComponent();
};
