// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DarkFortCharacter.h"
#include "Player/DarkFortPlayerState.h"
#include "DarkFortPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API ADarkFortPlayerCharacter : public ADarkFortCharacter
{
	GENERATED_BODY()
	

public: 
	ADarkFortPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

protected:

	bool bASCInputBound = false;

	FGameplayTag DeadTag;

	virtual void OnRep_PlayerState() override;
	
	void InitializeStartingValues(ADarkFortPlayerState* PS);

	void BindASCInput();
};
