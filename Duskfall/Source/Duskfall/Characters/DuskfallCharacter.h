// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterControls.h"
#include "DuskfallCharacter.generated.h"

UCLASS(config=Game)
class ADuskfallCharacter : public ACharacter, public ICharacterControls
{
	GENERATED_BODY()

public:
	ADuskfallCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/* Character control interface*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void JumpPressed();
		virtual void JumpPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void JumpReleased();
		virtual void JumpReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void AttackPressed();
		virtual void AttackPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void AttackReleased();
		virtual void AttackReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void BlockPressed();
		virtual void BlockPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void BlockReleased();
		virtual void BlockReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void DashPressed();
		virtual void DashPressed_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void DashReleased();
		virtual void DashReleased_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void MoveForward(float Scale);
		virtual void MoveForward_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void MoveRight(float Scale);
		virtual void MoveRight_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void TurnRate(float Scale);
		virtual void TurnRate_Implementation(float Scale) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Controls")
		void Turn(float Scale);
		virtual void Turn_Implementation(float Scale) override;

protected:
	virtual void BeginPlay();
};

