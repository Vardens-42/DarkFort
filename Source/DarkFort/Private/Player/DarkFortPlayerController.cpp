// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DarkFortPlayerController.h"
#include "Camera/DarkFortCameraManager.h"
#include "Player/DarkFortPlayerState.h"
#include "AbilitySystemComponent.h"

ADarkFortPlayerController::ADarkFortPlayerController()
{
	PlayerCameraManagerClass = ADarkFortCameraManager::StaticClass();
}
void ADarkFortPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ADarkFortPlayerState* PS = GetPlayerState<ADarkFortPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}