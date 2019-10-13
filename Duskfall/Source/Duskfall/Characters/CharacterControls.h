// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterControls.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCharacterControls : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSKFALL_API ICharacterControls
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Action mappings */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void JumpPressed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void JumpReleased();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void AttackPressed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void AttackReleased();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void BlockPressed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void BlockReleased();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void DashPressed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void DashReleased();

	/* Axis mappings */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void MoveForward(float Scale);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void MoveRight(float Scale);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void TurnRate(float Scale);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterControls")
		void Turn(float Scale);
};
