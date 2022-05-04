// Copyright Epic Games, Inc. All Rights Reserved.

#include "VampireALusthGameMode.h"
#include "VampireALusthCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVampireALusthGameMode::AVampireALusthGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
