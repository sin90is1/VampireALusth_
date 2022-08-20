// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilityType.h"
#include "VALPlayerController.generated.h"



UCLASS()
class VAMPIREALUSTH_API AVALPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerControlerBase")
	void AddAbilityToUI(FGameplayAbilityInfo AbilityInfo);

};