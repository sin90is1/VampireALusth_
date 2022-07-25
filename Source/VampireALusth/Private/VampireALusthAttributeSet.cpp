// Fill out your copyright notice in the Description page of Project Settings.


#include "VampireALusthAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void UVampireALusthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//Clamping Health based on Max Health.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}
