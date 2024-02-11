// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/DarkFortPlayerCharacter.h"
#include "Characters/DarkFortCharacterMovementComponent.h"
#include "DarkFort/Public/Player/DarkFortPlayerState.h"
#include "DarkFort/Public/Player/DarkFortPlayerController.h"
#include "Characters/Abilities/DarkFortAbilitySystemComponent.h"
#include "AI/DarkFortAIController.h"

ADarkFortPlayerCharacter::ADarkFortPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer.SetDefaultSubobjectClass<UDarkFortCharacterMovementComponent>(CharacterMovementComponentName))
{
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = ADarkFortAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

void ADarkFortPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADarkFortPlayerState* PS = GetPlayerState<ADarkFortPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		AddStartupEffects();
		AddCharacterAbilities();
	}

}

void ADarkFortPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADarkFortPlayerState* PS = GetPlayerState<ADarkFortPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		BindASCInput();
	}
}

void ADarkFortPlayerCharacter::InitializeStartingValues(ADarkFortPlayerState* PS)
{
	AbilitySystemComponent = Cast<UDarkFortAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AttributeSetBase = PS->GetAttributeSetBase();
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	InitializeAttributes();
	SetHealth(GetMaxHealth());
	SetStamina(GetMaxStamina());
}
void ADarkFortPlayerCharacter::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("EDarkFortAbilityInputID"), static_cast<int32>(EDarkFortAbilityInputID::Confirm), static_cast<int32>(EDarkFortAbilityInputID::Cancel)));
		bASCInputBound = true;
	}
}
