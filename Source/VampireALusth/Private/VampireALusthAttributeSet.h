// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VampireALusthAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHelthchangeDelegate, float, Health, float, MaxHealth);

UCLASS()
class UVampireALusthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UVampireALusthAttributeSet();

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetCharacter")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Character")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Character")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, Mana)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Character")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, MaxMana)


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Character")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, Energy)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Character")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UVampireALusthAttributeSet, MaxEnergy)


		FOnHelthchangeDelegate OnHelthchange;
		FOnHelthchangeDelegate OnManachange;
		FOnHelthchangeDelegate OnEnergychange;

};