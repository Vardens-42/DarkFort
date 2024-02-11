
// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/DarkFortWaitReceiveDamage.h"
#include "Characters/Abilities/DarkFortAbilitySystemComponent.h"

UDarkFortWaitReceiveDamage::UDarkFortWaitReceiveDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TriggerOnce = false;
}

UDarkFortWaitReceiveDamage* UDarkFortWaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	UDarkFortWaitReceiveDamage* MyObj = NewAbilityTask<UDarkFortWaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UDarkFortWaitReceiveDamage::Activate()
{
	UDarkFortAbilitySystemComponent* DFASC = Cast<UDarkFortAbilitySystemComponent>(AbilitySystemComponent);

	if (DFASC)
	{
		DFASC->ReceivedDamage.AddDynamic(this, &UDarkFortWaitReceiveDamage::OnDamageReceived);
	}
}

void UDarkFortWaitReceiveDamage::OnDestroy(bool AbilityIsEnding)
{
	UDarkFortAbilitySystemComponent* DFASC = Cast<UDarkFortAbilitySystemComponent>(AbilitySystemComponent);

	if (DFASC)
	{
		DFASC->ReceivedDamage.RemoveDynamic(this, &UDarkFortWaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UDarkFortWaitReceiveDamage::OnDamageReceived(UDarkFortAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}