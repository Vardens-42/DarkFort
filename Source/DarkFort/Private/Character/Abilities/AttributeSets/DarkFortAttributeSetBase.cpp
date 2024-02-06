// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/DarkFortAttributeSetBase.h"
#include "Net/UnrealNetwork.h"

void UDarkFortAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void UDarkFortAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSetBase, Health, OldHealth);
}

void UDarkFortAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UDarkFortAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSetBase, Stamina, OldStamina);
}

void UDarkFortAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkFortAttributeSetBase, MaxStamina, OldMaxStamina);
}

void UDarkFortAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDarkFortAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
}