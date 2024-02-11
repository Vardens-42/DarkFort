// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/DarkFortPlayerCharacter.h"
#include "Characters/DarkFortCharacterMovementComponent.h"
#include "Player/DarkFortPlayerState.h"
#include "Player/DarkFortPlayerController.h"
#include "Characters/Abilities/DarkFortAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/DarkFortAttributeSet.h"
#include "AI/DarkFortAIController.h"

ADarkFortPlayerCharacter::ADarkFortPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = ADarkFortAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

void ADarkFortPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void ADarkFortPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADarkFortPlayerState* PS = GetPlayerState<ADarkFortPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UDarkFortAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		AttributeSetBase = PS->GetAttributeSetBase();
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		InitializeAttributes();
		SetHealth(GetMaxHealth());
		SetStamina(GetMaxStamina());
		AddStartupEffects();
		AddCharacterAbilities();
	}

}

void ADarkFortPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADarkFortPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADarkFortPlayerState* PS = GetPlayerState<ADarkFortPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UDarkFortAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetStamina(GetMaxStamina());
	}
}
void ADarkFortPlayerCharacter::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/DarkFort"), FName("EDarkFortAbilityInputID"));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
		FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EDarkFortAbilityInputID::Confirm), static_cast<int32>(EDarkFortAbilityInputID::Cancel)));
	
		bASCInputBound = true;
	}
}
