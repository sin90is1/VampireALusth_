// Copyright Epic Games, Inc. All Rights Reserved.

#include "VampireALusthCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameplayAbilityBase.h"
#include "VALPlayerController.h"


//////////////////////////////////////////////////////////////////////////
// AVampireALusthCharacter


AVampireALusthCharacter::AVampireALusthCharacter()
{
	//9 setup AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;


	AttributeSet = CreateDefaultSubobject<UVampireALusthAttributeSet>(TEXT("AttributeSet"));

	bIsDead = false;
	TeamID = 255;

}


void AVampireALusthCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributeSet->OnHelthchange.AddDynamic(this, &AVampireALusthCharacter::OnHealthChanged);
	AttributeSet->OnManachange.AddDynamic(this, &AVampireALusthCharacter::OnManaChanged);
	AttributeSet->OnEnergychange.AddDynamic(this, &AVampireALusthCharacter::OnEnergyChanged);

	AutoDeterminTeamIDbyControllerType();
	AddGameplayTag(FullHealthTag);
}



float AVampireALusthCharacter::GetHealth() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetHealth();
	}

	return -1.0f;
}

float AVampireALusthCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}



// void AVampireALusthCharacter::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode)
// {
// 
// 	if (GetLocalRole() == ROLE_Authority && IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
// 	{
// 		UGameplayAbility* Ability = AbilityClass->GetDefaultObject<UGameplayAbility>();
// 
// 		if (IsValid(Ability))
// 		{
// 			FGameplayAbilitySpec AbilitySpec(
// 				Ability,
// 				Level,
// 				InputCode
// 			);
// 
// 			AbilitySystemComponent->GiveAbility(AbilitySpec);
// 		}
// 	}
// }

void AVampireALusthCharacter::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1, 0));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AVampireALusthCharacter::AquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityToAquire)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilityToAquire )
	{
		AquireAbility(AbilityItem);
		if (AbilityItem->IsChildOf(UGameplayAbilityBase::StaticClass()))
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
			if (AbilityBaseClass != nullptr)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}

void AVampireALusthCharacter::ActivateAbility(int32 InputCode)
{

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
	}
}



void AVampireALusthCharacter::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health<=0.0f && !bIsDead)
	{
		bIsDead = true;
		Dead();
		BP_Die();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

void AVampireALusthCharacter::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void AVampireALusthCharacter::OnEnergyChanged(float Energy, float MaxEnergy)
{
	BP_OnEnergyChanged(Energy, MaxEnergy);
}

bool AVampireALusthCharacter::IsOtherHostile(AVampireALusthCharacter* Other)
 {
 	return TeamID != Other->GetTeamID();
 }
 
 uint8 AVampireALusthCharacter::GetTeamID() const
 {
 	return TeamID;
 }

 void AVampireALusthCharacter::AddGameplayTag(FGameplayTag& TagToAdd)
 {
	 GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	 GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
 }

 void AVampireALusthCharacter::RemoveGameplayTag(FGameplayTag& TagToRemove)
 {
	 GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
 }

 void AVampireALusthCharacter::AutoDeterminTeamIDbyControllerType()
 {
 	if (GetController()&& GetController()->IsPlayerController())
 	{
 		TeamID = 0;
 	}
 }

void AVampireALusthCharacter::Dead()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->DisableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Dead");
	}
}

void AVampireALusthCharacter::DisableInputControl()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->DisableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Dead");
	}
}

void AVampireALusthCharacter::EnableInputControl()
{
	if (!bIsDead)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		if (PC)
		{
			PC->EnableInput(PC);
		}
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			AIC->GetBrainComponent()->RestartLogic();
		}
	}
	
}

void AVampireALusthCharacter::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	AVALPlayerController* PlayerControlerBase = Cast<AVALPlayerController>(GetController());
	if (PlayerControlerBase)
	{
		UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			PlayerControlerBase->AddAbilityToUI(AbilityInfo);
		}
	}
}

void AVampireALusthCharacter::HitStun(float StunDuration)
{
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &AVampireALusthCharacter::EnableInputControl, StunDuration, false);
}