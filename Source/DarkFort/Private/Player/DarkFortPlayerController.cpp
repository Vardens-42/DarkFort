// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DarkFortPlayerController.h"
#include "Player/DarkFortPlayerState.h"

void ADarkFortPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ADarkFortPlayerState* PState = GetPlayerState<ADarkFortPlayerState>();

	if (PState)
	{
		PState->GetAbilitySystemComponent()->InitAbilityActorInfo(PState, InPawn);
	}
}

ADarkFortPlayerController::ADarkFortPlayerController()
{
	PlayerCameraManagerClass = ADarkFortCameraManager::StaticClass();
};