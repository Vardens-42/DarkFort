// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/AttributeSets/DarkFortAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
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

	class UDarkFortAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Dark Fort|Player State|Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UDarkFortAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UDarkFortAttributeSet* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

};
