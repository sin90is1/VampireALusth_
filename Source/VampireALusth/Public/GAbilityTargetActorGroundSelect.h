// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GAbilityTargetActorGroundSelect.generated.h"

class UDecalComponent;
class USceneComponent;
/**
 * 
 */
UCLASS()
class VAMPIREALUSTH_API AGAbilityTargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	

public:
	AGAbilityTargetActorGroundSelect();
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = "GraoundSelect")
	bool GetPlayerLookingPoint(FVector& OutViewPoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true), Category = "GroundSelect")
	float Radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundSelect")
		UDecalComponent* Decal;

	USceneComponent* RootComp;
};
