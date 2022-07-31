// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonFPSGameMode.h"
#include "ThirdPersonFPS/Player/ThirdPersonFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonFPSGameMode::AThirdPersonFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
