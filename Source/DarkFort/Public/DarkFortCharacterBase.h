// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DarkFort.h"
#include "DarkFortCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ADarkFortCharacterBase*, character);

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADarkFortCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement) class UDarkFortCharacterMovementComponent* DarkFortCharacterMovementComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Walk Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WalkAction;

public:
	bool bPressedDarkFortJump;

public:
	ADarkFortCharacterBase(const class FObjectInitializer& ObjectInitializer);
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;
	
protected:

	virtual void Jump() override;
	virtual void StopJumping() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
		
	void StartSprint();

	void StopSprinting();

	void ToggleCrouch();

	void ToggleWalk();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	FORCEINLINE UDarkFortCharacterMovementComponent* GetDarkFortCharacterMovement() const {
		return DarkFortCharacterMovementComponent;
	}

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual int32 GetAbilityLevel(DarkFortAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxStamina() const;

protected:
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UDarkFortAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Animation")
	TArray <TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);

};