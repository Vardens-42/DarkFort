// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkFortAssetManager.h"
#include "AbilitySystemGlobals.h"

UDarkFortAssetManager& UDarkFortAssetManager::Get() 
{
	UDarkFortAssetManager* Singleton = Cast<UDarkFortAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be DarkFortAssetManager!"));
		return *NewObject<UDarkFortAssetManager>();	 // never calls this
	}
}

void UDarkFortAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
