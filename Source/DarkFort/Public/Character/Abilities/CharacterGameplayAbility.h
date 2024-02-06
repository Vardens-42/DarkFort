// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DarkFort.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCharacterGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	DarkFortAbilityID AbilityInputID = DarkFortAbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	DarkFortAbilityID AbilityID = DarkFortAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
