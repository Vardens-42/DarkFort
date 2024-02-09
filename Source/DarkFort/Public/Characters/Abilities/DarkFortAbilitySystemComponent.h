// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DarkFortAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UDarkFortAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);


/**
 * 
 */
UCLASS()
class DARKFORT_API UDarkFortAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UDarkFortAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
