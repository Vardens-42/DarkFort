// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkFortCameraManager.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DarkFortPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API ADarkFortPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADarkFortPlayerController();

	virtual void OnPossess(APawn* InPawn) override;
};
