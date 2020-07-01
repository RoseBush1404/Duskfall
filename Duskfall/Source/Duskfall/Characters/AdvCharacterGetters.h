// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AdvCharacterGetters.generated.h"

class UCharacterMovementComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAdvCharacterGetters : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSKFALL_API IAdvCharacterGetters
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterGetters")
		UCharacterMovementComponent* GetCharacterMovementComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterGetters")
		APlayerCameraManager* GetPlayerCameraManager();
};
