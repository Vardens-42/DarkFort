// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/DarkFortAbilitySystemComponent.h"

void UDarkFortAbilitySystemComponent::ReceiveDamage(UDarkFortAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
