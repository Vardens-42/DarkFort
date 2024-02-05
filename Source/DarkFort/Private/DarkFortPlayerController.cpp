// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkFortPlayerController.h"
#include "DarkFortCameraManager.h"

ADarkFortPlayerController::ADarkFortPlayerController()
{
	PlayerCameraManagerClass = ADarkFortCameraManager::StaticClass();
};