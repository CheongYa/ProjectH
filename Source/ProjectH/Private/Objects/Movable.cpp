// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Movable.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/PushComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMovable::AMovable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AMovable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<UArrowComponent*> ExistingArrows;
	GetComponents<UArrowComponent>(ExistingArrows);

	for (UArrowComponent* Arrow : ExistingArrows)
	{
		Arrow->DestroyComponent();
	}

	for (const auto& PushTransform : PushTransforms)
	{
		UArrowComponent* ArrowComponent = NewObject<UArrowComponent>(this);
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->SetRelativeTransform(PushTransform);
		ArrowComponent->RegisterComponent();
	}
}

void AMovable::Hold(APlayerCharacter* Character)
{
	if (!IsValid(Character)) return;
	
	UPushComponent* PushComponent = Character->GetPushComponent();
	if (!IsValid(PushComponent)) return;
	
	const FTransform CharacterTransform = Character->GetActorTransform();

	int32 Index = FindClosestPushTransformIndex(FVector2D(CharacterTransform.GetLocation()), PushComponent->GetPushRange());
	if (Index >= 0)
	{
		const FTransform TargetTransform = PushTransforms[Index] * GetActorTransform();

		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		if (IsValid(CapsuleComponent))
		{
			const FVector AddVector = TargetTransform.GetLocation() + FVector(
				0.f, 0.f, CapsuleComponent->GetScaledCapsuleHalfHeight());
			FTransform NewTransform = FTransform(TargetTransform.GetRotation().Rotator(), AddVector,
			                                     CharacterTransform.GetScale3D());

			const FVector Start = NewTransform.GetLocation() + FVector(0.f, 0.f, 70.f);
			const FVector End = NewTransform.GetLocation() - FVector(0.f, 0.f, 100.f);

			FHitResult HitResult;

			bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
				GetWorld(),
				Start,
				End,
				CapsuleComponent->GetScaledCapsuleRadius(),
				CapsuleComponent->GetScaledCapsuleHalfHeight(),
				UEngineTypes::ConvertToTraceType(ECC_Visibility),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::ForDuration,
				HitResult,
				true
			);

			if (bHit)
			{
				if (!HitResult.bStartPenetrating)
				{
					UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
					if (IsValid(MovementComponent))
					{
						float FloorZ = MovementComponent->GetWalkableFloorZ();
						if (FloorZ < HitResult.ImpactNormal.Z)
						{
							TArray<AActor*> IgnoredActors;
							IgnoredActors.Add(Character);

							FHitResult HitResult2;

							bool bHit2 = UKismetSystemLibrary::LineTraceSingle(
								GetWorld(),
								GetActorLocation(),
								NewTransform.GetLocation(),
								UEngineTypes::ConvertToTraceType(ECC_Visibility),
								false,
								IgnoredActors,
								EDrawDebugTrace::None,
								HitResult2,
								true
							);

							if (!bHit2)
							{
								Character->SetActorTransform(NewTransform);
								PushComponent->BeginPush(this);
							}
							else
							{
								UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WallsBetween Character and Object"));
							}
						}
					}
					else
					{
						UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Floor is Not Walkable"));
					}
				}
				else
				{
					UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Room to Stand"));
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AMovable::BeginPlay()
{
	Super::BeginPlay();
}

int32 AMovable::FindClosestPushTransformIndex(const FVector2D& CharacterLocation, float PushRange)
{
	float ClosestDistance = 0;
	int32 ClosestTransformIndex = -1;

	for (int32 i = 0; i < PushTransforms.Num(); ++i)
	{
		const FVector Vector = GetActorTransform().TransformPosition(PushTransforms[i].GetLocation());
		double Distance = FVector2D::DistSquared(FVector2D(Vector), CharacterLocation);
		if (Distance < PushRange * PushRange)
		{
			if (Distance < ClosestDistance || ClosestTransformIndex < 0)
			{
				ClosestTransformIndex = i;
				ClosestDistance = Distance;
			}
		}
	}

	return ClosestTransformIndex;
}

// Called every frame
void AMovable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
