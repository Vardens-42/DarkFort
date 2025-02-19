// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/DarkFortGameMode.h"
#include "Characters/DarkFortCharacter.h"
#include "Player/DarkFortPlayerController.h"
#include "Player/DarkFortPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ADarkFortGameMode::ADarkFortGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/DarkFort/Characters/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ADarkFortPlayerController::StaticClass();
	PlayerStateClass = ADarkFortPlayerState::StaticClass();
}
