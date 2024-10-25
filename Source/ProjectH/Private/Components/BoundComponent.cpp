// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoundComponent.h"

// Sets default values for this component's properties
UBoundComponent::UBoundComponent()
	: CurrentHeight(0.0f), bIsUp(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBoundComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}


// Called every frame
void UBoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	
	if(bIsUp) {
		CurrentHeight += DeltaTime;
		Owner->SetActorLocation(Owner->GetActorLocation() + FVector(0.0f, 0.0f, CurrentHeight));
		if(CurrentHeight >= Height) {
			bIsUp = false;
			CurrentHeight = 0.0f;
		}
	}
	else {
		CurrentHeight -= DeltaTime;
		Owner->SetActorLocation(Owner->GetActorLocation() + FVector(0.0f, 0.0f, CurrentHeight));
		if(CurrentHeight <= -Height) {
			bIsUp = true;
			CurrentHeight = 0.0f;
		}
	}
}

