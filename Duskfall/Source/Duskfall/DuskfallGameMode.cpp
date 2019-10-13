// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DuskfallGameMode.h"
#include "DuskfallHUD.h"
#include "Characters/DuskfallCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADuskfallGameMode::ADuskfallGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Characters/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADuskfallHUD::StaticClass();
}
