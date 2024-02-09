// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkFortCharacterBase.h"
#include "Player/DarkFortPlayerState.h"
#include "DarkFortPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API ADarkFortPlayerCharacter : public ADarkFortCharacterBase
{
	GENERATED_BODY()
	
public:
	ADarkFortPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	void InitializeStartingProperties(ADarkFortPlayerState* PState);
	void OnRep_PlayerState();
};
