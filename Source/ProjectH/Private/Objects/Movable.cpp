// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Movable.h"

#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

void AMovable::Hold(const FVector& Location)
{
	const FTransform ActorTransform = GetActorTransform();

	float ClosestDistance = 0;
	int32 ClosestTransformIndex = -1;
	
	for (int32 i = 0; i < PushTransforms.Num(); ++i)
	{
		const FVector Vector = UKismetMathLibrary::TransformLocation(ActorTransform, PushTransforms[i].GetLocation());
		double Distance = UKismetMathLibrary::DistanceSquared2D(FVector2D(Vector), FVector2D(Location));
		if (Distance < 10000)
		{
			if (Distance < ClosestDistance || ClosestTransformIndex < 0)
			{
				ClosestTransformIndex = i;
				ClosestDistance = Distance;
			}
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString::Printf(TEXT("ClosestTransformIndex: %d"), ClosestTransformIndex));
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
