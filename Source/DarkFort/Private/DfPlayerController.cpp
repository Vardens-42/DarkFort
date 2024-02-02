// Fill out your copyright notice in the Description page of Project Settings.


#include "DfPlayerController.h"
#include "DFCameraManager.h"

ADfPlayerController::ADfPlayerController()
{
	PlayerCameraManagerClass = ADFCameraManager::StaticClass();
};