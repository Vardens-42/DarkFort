// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkFortAssetManager.h"
#include "AbilitySystemGlobals.h"

void UDarkFortAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
