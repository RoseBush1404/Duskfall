// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UHealthSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSKFALL_API IHealthSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthSystem")
		void TakeDamage(float Damage, float DamageMoifier, AActor* DamageCauser);
};
