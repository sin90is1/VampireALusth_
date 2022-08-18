// Fill out your copyright notice in the Description page of Project Settings.


#include "VampireALusthAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "F:\UE5_test\VampireALusth\Source\VampireALusth\VampireALusthCharacter.h"

UVampireALusthAttributeSet::UVampireALusthAttributeSet()
	 :MaxHealth(200.f),
	 Health(200.f),
	 Mana(100.0f),
	 MaxMana(100.0f),
	 Energy(250.0f),
	 MaxEnergy(250.0f)
{

}

void UVampireALusthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//Clamping Health based on Max Health.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		OnHelthchange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
		AVampireALusthCharacter* CharacterOwner = Cast<AVampireALusthCharacter>(GetOwningActor());
		if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue())
		{
			if (CharacterOwner)
			{
				CharacterOwner->AddGameplayTag(CharacterOwner->FullHealthTag);
			}
		}
		else
		{
			if (CharacterOwner)
			{
				CharacterOwner->RemoveGameplayTag(CharacterOwner->FullHealthTag);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManachange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		Energy.SetCurrentValue(FMath::Clamp(Energy.GetCurrentValue(), 0.f, MaxEnergy.GetCurrentValue()));
		Energy.SetBaseValue(FMath::Clamp(Energy.GetBaseValue(), 0.f, MaxEnergy.GetCurrentValue()));
		OnEnergychange.Broadcast(Energy.GetCurrentValue(), MaxEnergy.GetCurrentValue());
	}
}
