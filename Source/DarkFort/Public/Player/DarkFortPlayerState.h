// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Character/Abilities/AttributeSets/DarkFortAttributeSetBase.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "DarkFortPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DARKFORT_API ADarkFortPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADarkFortPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UDarkFortAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Player State|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Player State|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player State|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player State|Attributes")
	float GetStamina() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player State|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Player State|Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UCharacterAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class UDarkFortAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle MaxHealthChangeDelegateHandle;
	FDelegateHandle StaminaChangeDelegateHandle;
	FDelegateHandle MaxStaminaChangeDelegateHandle;
	FDelegateHandle CharacterLevelChangeDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
