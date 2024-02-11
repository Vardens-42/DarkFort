// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DarkFortPlayerState.h"
#include "Characters/Abilities/AttributeSets/DarkFortAttributeSet.h"
#include "Characters/Abilities/DarkFortAbilitySystemComponent.h"

ADarkFortPlayerState::ADarkFortPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDarkFortAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UDarkFortAttributeSet>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ADarkFortPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UDarkFortAttributeSet* ADarkFortPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ADarkFortPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ADarkFortPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	//TODO
}

float ADarkFortPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ADarkFortPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ADarkFortPlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float ADarkFortPlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

int32 ADarkFortPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

void ADarkFortPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADarkFortPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ADarkFortPlayerState::MaxHealthChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ADarkFortPlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &ADarkFortPlayerState::MaxStaminaChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADarkFortPlayerState::CharacterLevelChanged);
	
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ADarkFortPlayerState::StunTagChanged);
	}
}

void ADarkFortPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}

void ADarkFortPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed!"));
}

void ADarkFortPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Stamina Changed!"));
}

void ADarkFortPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Stamina Changed!"));
}

void ADarkFortPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Level Changed!"));
}

void ADarkFortPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
