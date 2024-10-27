// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Movable.h"

#include "Character/CharacterBase.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

void AMovable::Hold(const ACharacterBase* Character)
{
	if (!IsValid(Character)) return;
	
	const FTransform ActorTransform = GetActorTransform();
	const FTransform CharacterTransform = Character->GetActorTransform();

	float ClosestDistance = 0;
	int32 ClosestTransformIndex = -1;
	
	for (int32 i = 0; i < PushTransforms.Num(); ++i)
	{
		const FVector Vector = UKismetMathLibrary::TransformLocation(ActorTransform, PushTransforms[i].GetLocation());
		double Distance = UKismetMathLibrary::DistanceSquared2D(FVector2D(Vector), FVector2D(CharacterTransform.GetLocation()));
		if (Distance < 10000)
		{
			if (Distance < ClosestDistance || ClosestTransformIndex < 0)
			{
				ClosestTransformIndex = i;
				ClosestDistance = Distance;
			}
		}
	}

	const FTransform TargetTransform = UKismetMathLibrary::ComposeTransforms(PushTransforms[ClosestTransformIndex], ActorTransform);
	
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if (IsValid(CapsuleComponent))
	{
		const FVector AddVector = UKismetMathLibrary::Add_VectorVector(TargetTransform.GetLocation(), FVector(0.f, 0.f, CapsuleComponent->GetScaledCapsuleHalfHeight()));
		FTransform NewTransform = UKismetMathLibrary::MakeTransform(AddVector, TargetTransform.GetRotation().Rotator(), CharacterTransform.GetScale3D());

		const FVector Start = UKismetMathLibrary::Add_VectorVector(NewTransform.GetLocation(), FVector(0.f, 0.f, 70.f));
		const FVector End = UKismetMathLibrary::Add_VectorVector(NewTransform.GetLocation(), FVector(0.f, 0.f, 100.f));

		FHitResult HitResult;
		
		UKismetSystemLibrary::CapsuleTraceSingle(
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
	}
}

// Called when the game starts or when spawned
void AMovable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
