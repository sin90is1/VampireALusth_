// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CooldownEffect && CostEffect)
	{
		float CooldownDuration = 0;
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration);
		float Cost = 0;
		EAbilityCostType CostType;
		if (CostEffect->Modifiers.Num()>0)
		{
			FGameplayModifierInfo EffectInfo = CostEffect->Modifiers[0];
			EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
			FGameplayAttribute CostAttribute = EffectInfo.Attribute;
			FString AttributeName = CostAttribute.AttributeName;
			if (AttributeName == "Health")
			{
				CostType = EAbilityCostType::Health;
			}
			else if (AttributeName == "Mana") 
			{
				CostType = EAbilityCostType::Mana;
			}
			else if (AttributeName == "Energy")
			{
				CostType = EAbilityCostType::Energy;
			}
			return FGameplayAbilityInfo(CooldownDuration, Cost, CostType, UIMaterial, GetClass());
		}
	}
	return FGameplayAbilityInfo();
}
