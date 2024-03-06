// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyRunnerGameMode.h"
#include "EasyRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEasyRunnerGameMode::AEasyRunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
