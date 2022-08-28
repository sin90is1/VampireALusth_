// Fill out your copyright notice in the Description page of Project Settings.


#include "GAbilityTargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components\DecalComponent.h"
#include "Components\SceneComponent.h"

AGAbilityTargetActorGroundSelect::AGAbilityTargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;
	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);
	Decal->SetupAttachment(RootComp);
	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);
}

void AGAbilityTargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
}

void AGAbilityTargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	bool TraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* Masterpawn = MasterPC->GetPawn();
	if (Masterpawn)
	{
		CollisionQueryParams.AddIgnoredActor(Masterpawn->GetUniqueID());
	}

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps, 
		ViewLocation, 
		FQuat::Identity, 
		FCollisionObjectQueryParams(ECC_Pawn), 
		FCollisionShape::MakeSphere(Radius), 
		CollisionQueryParams);
	if (TryOverlap)
	{
		for (int32 i=0; i<Overlaps.Num(); ++i)
		{
			APawn* PawnOverlaped = Cast<APawn>(Overlaps[i].GetActor());
			if (PawnOverlaped && !OverlapedActors.Contains(PawnOverlaped))
			{
				OverlapedActors.Add(PawnOverlaped);
			}
		}
	}

	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}
	
	if (OverlapedActors.Num()>0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation));
	}
}


void AGAbilityTargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);
	Decal->SetWorldLocation(LookPoint);
}

bool AGAbilityTargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector Viewpoint;
	FRotator ViewRoatation;
	MasterPC->GetPlayerViewPoint(Viewpoint, ViewRoatation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* Masterpawn = MasterPC->GetPawn();
	if (MasterPC)
	{
		QueryParams.AddIgnoredActor(Masterpawn->GetUniqueID());
	}
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, Viewpoint, Viewpoint + ViewRoatation.Vector() * 10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		OutViewPoint = HitResult.ImpactPoint;
	}
	else
	{
		OutViewPoint = FVector();
	}
	return TryTrace;
}
