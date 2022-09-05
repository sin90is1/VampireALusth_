// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "VampireALusthAttributeSet.h"
#include "VampireALusthCharacter.generated.h"

class UVampireALusthAttributeSet;
class UGameplayAbilityBase;

UCLASS(config=Game)
class AVampireALusthCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AVampireALusthCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
		class UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes|Abilities")
	virtual	float GetHealth() const;

 	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes|Abilities")
	virtual	float GetMaxHealth() const;

	/** Grants an Ability at the given level */
// 	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
// 		void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode);

	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
		void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
		void AquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityToAquire);

	/*activate an Ability whit the matching input code*/
	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
		void ActivateAbility(int32 InputCode);

	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttributeSetCharacter", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
		void OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttributeSetCharacter", meta = (DisplayName = "OnManaChanged"))
		void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION()
		void OnEnergyChanged(float Energy, float MaxEnergy);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttributeSetCharacter", meta = (DisplayName = "OnEnergyChanged"))
		void BP_OnEnergyChanged(float Energy, float MaxEnergy);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttributeSetCharacter", meta = (DisplayName = "Die"))
		void BP_Die();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		bool IsOtherHostile(AVampireALusthCharacter* Other);

	uint8 GetTeamID() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void AddGameplayTag(FGameplayTag& TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void RemoveGameplayTag(FGameplayTag& TagToRemove);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		FGameplayTag FullHealthTag;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void HitStun(float StunDuration);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void ApplyGESpectHandleTargetDataSpecsHandle(const FGameplayEffectSpecHandle& GESpectHandle, const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttributeSetCharacter")
		UVampireALusthAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

protected:


 	bool bIsDead;
 	uint8 TeamID;
	void AutoDeterminTeamIDbyControllerType();
	void Dead();
	void DisableInputControl();
	void EnableInputControl();
	FTimerHandle StunTimeHandle;
	void AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd);


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

