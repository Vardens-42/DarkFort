// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/AttributeSets/DarkFortAttributeSet.h"
#include "Net/UnrealNetwork.h"

UDarkFortAttributeSet::UDarkFortAttributeSet()
{
}

void UDarkFortAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
}

void UDarkFortAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSet, Health, OldHealth);
}

void UDarkFortAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSet, MaxHealth, OldMaxHealth);
}

void UDarkFortAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSet, Stamina, OldStamina);
}

void UDarkFortAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSet, MaxStamina, OldMaxStamina);
}

void UDarkFortAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSet, CharacterLevel, OldCharacterLevel);
}

