// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DarkFort.h"
#include "DarkFortGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API UDarkFortGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	UDarkFortGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EDarkFortAbilityInputID AbilityInputID = EDarkFortAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EDarkFortAbilityInputID AbilityID = EDarkFortAbilityInputID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

};
