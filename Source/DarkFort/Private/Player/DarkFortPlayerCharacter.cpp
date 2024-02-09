// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DarkFortPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/DarkFortPlayerState.h"
#include "AI/PlayerAIController.h"
#include "Player/DarkFortPlayerController.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"

ADarkFortPlayerCharacter::ADarkFortPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = APlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void ADarkFortPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADarkFortPlayerState* PState = GetPlayerState<ADarkFortPlayerState>();
	if (PState)
	{
		InitializeStartingProperties(PState);
		AddStartupEffects();
		AddCharacterAbilities();
	}
}

void ADarkFortPlayerCharacter::InitializeStartingProperties(ADarkFortPlayerState* PState)
{
	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PState->GetAbilitySystemComponent());
	
	PState->GetAbilitySystemComponent()->InitAbilityActorInfo(PState, this);

	AttributeSetBase = PState->GetAttributeSetBase();
	
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	InitializeAttributes();
	SetHealth(GetMaxHealth());
	SetStamina(GetMaxStamina());


}

void ADarkFortPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADarkFortPlayerState* PState = GetPlayerState<ADarkFortPlayerState>();
	if (PState)
	{
		InitializeStartingProperties(PState);
	}
}
