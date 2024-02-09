// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DarkFortPlayerController.h"
#include "Camera/DarkFortCameraManager.h"

ADarkFortPlayerController::ADarkFortPlayerController()
{
	PlayerCameraManagerClass = ADarkFortCameraManager::StaticClass();
};