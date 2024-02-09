// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkFortGameMode.h"
#include "DarkFortCharacterBase.h"
#include "Player/DarkFortPlayerController.h"
#include "Player/DarkFortPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ADarkFortGameMode::ADarkFortGameMode()
{
	PlayerControllerClass = ADarkFortPlayerController::StaticClass();
	PlayerStateClass = ADarkFortPlayerState::StaticClass();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_DarkFortPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}



}
