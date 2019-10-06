// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuskfallCharacter.generated.h"

UCLASS(config=Game)
class ADuskfallCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADuskfallCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

protected:
	virtual void BeginPlay();
	
	/** Fires a projectile. */
	virtual void OnFire();
};

