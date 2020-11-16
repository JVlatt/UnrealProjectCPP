// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodeGameMode.h"
#include "CodeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACodeGameMode::ACodeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
