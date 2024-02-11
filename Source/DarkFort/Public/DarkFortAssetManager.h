// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DarkFortAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API UDarkFortAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UDarkFortAssetManager& Get();

	virtual void StartInitialLoading() override;
};
