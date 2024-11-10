// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/PushComponent.h"

#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objects/Movable.h"

// Sets default values for this component's properties
UPushComponent::UPushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bCanMoveRight = true;
	bCanMoveLeft = true;
	bCanMoveUp = true;
	bCanMoveDown = true;
}

void UPushComponent::BeginPush(AMovable* Movable)
{
	if (IsValid(PlayerCharacter) && !IsValid(MovableObject))
	{
		MovableObject = Movable;
		PlayerCharacter->AttachToActor(MovableObject, FAttachmentTransformRules::KeepWorldTransform);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		SetComponentTickEnabled(true);
		MovableObject->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void UPushComponent::EndPush()
{
	bCanMoveRight = true;
	bCanMoveLeft = true;
	bCanMoveUp = true;
	bCanMoveDown = true;
	
	if (IsValid(PlayerCharacter) && IsValid(MovableObject))
	{
		MovableObject->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		bIsMovingAnObject = false;
		PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		SetComponentTickEnabled(false);

		MovableObject = nullptr;

		ForwardMove = 0.f;
		RightMove = 0.f;
	}
}

bool UPushComponent::IsHoldingObject() const
{
	return IsValid(MovableObject);
}

FVector UPushComponent::GetDeltaLocation(float InDeltaSecond, float InForwardMove, float InRightMove)
{
	if (InForwardMove != 0.f || InRightMove != 0.f)
	{
		bIsMovingAnObject = true;

		const FVector ForwardVector = (PlayerCharacter->GetActorForwardVector() * InForwardMove) * (InDeltaSecond * 100.f);
		const FVector RightVector = (PlayerCharacter->GetActorRightVector() * InRightMove) * (InDeltaSecond * 100.f);
		return ForwardVector + RightVector;
	}

	return FVector::ZeroVector;
}


// Called when the game starts
void UPushComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetComponentTickEnabled(false);

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
}


// Called every frame
void UPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	const FVector Start = PlayerCharacter->GetActorLocation();
	const FRotator PlayerRotation = PlayerCharacter->GetActorRotation();
	
	if (!bCanMoveRight)
	{
		ForwardMove = FMath::Clamp(ForwardMove, 0.f, 1.f);
	}
	
	if (!bCanMoveLeft)
	{
		ForwardMove = FMath::Clamp(ForwardMove, -1.f, 0.f);
	}
	
	if (!bCanMoveUp)
	{
		RightMove = FMath::Clamp(RightMove, 0.f, 1.f);
	}
	
	if (!bCanMoveDown)
	{
		RightMove = FMath::Clamp(RightMove, -1.f, 0.f);
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter);
	
	if (IsHoldingObject())
	{
		CurrentActorZRotation = PlayerCharacter->GetActorRotation().Yaw;
		if (CurrentActorZRotation == -90.f) // Up
		{
			const FVector DeltaLocation = GetDeltaLocation(DeltaTime, RightMove * -1.f, ForwardMove);
			MovableObject->AddActorWorldOffset(DeltaLocation, true);
			
			{
				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
					GetWorld(),
					PlayerCharacter->GetActorLocation(),
					PlayerCharacter->GetActorLocation(),
					42.f,
					90.f,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveUp = !bHit;
			}
			
			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 180.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveRight = !bHit;
			}

			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 0.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
                    GetWorld(),
                    Start,
                    End,
                    UEngineTypes::ConvertToTraceType(ECC_Visibility),
                    false,
                    ActorsToIgnore,
                    EDrawDebugTrace::ForOneFrame,
                    HitResult,
                    true
                );

				bCanMoveLeft = !bHit;
			}
		}
		if (CurrentActorZRotation == 0.f) // Right
		{
			const FVector DeltaLocation = GetDeltaLocation(DeltaTime, ForwardMove, RightMove);
			MovableObject->AddActorWorldOffset(DeltaLocation, true);
			
			{
				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
					GetWorld(),
					PlayerCharacter->GetActorLocation(),
					PlayerCharacter->GetActorLocation(),
					42.f,
					90.f,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveRight = !bHit;
			}
			
			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, -90.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveUp = !bHit;
			}

			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 90.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveDown = !bHit;
			}
		}
		if (CurrentActorZRotation == 180.f) // Left
		{
			const FVector DeltaLocation = GetDeltaLocation(DeltaTime, ForwardMove * -1.f, RightMove * -1.f);
			MovableObject->AddActorWorldOffset(DeltaLocation, true);
			
			{
				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
					GetWorld(),
					PlayerCharacter->GetActorLocation(),
					PlayerCharacter->GetActorLocation(),
					42.f,
					90.f,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveLeft = !bHit;
			}
			
			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, -90.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveUp = !bHit;
			}

			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 90.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveDown = !bHit;
			}
		}
		if (CurrentActorZRotation == 90.f) // Down
		{
			const FVector DeltaLocation = GetDeltaLocation(DeltaTime, RightMove, ForwardMove * -1.f);
			MovableObject->AddActorWorldOffset(DeltaLocation, true);
			
			{
				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
					GetWorld(),
					PlayerCharacter->GetActorLocation(),
					PlayerCharacter->GetActorLocation(),
					42.f,
					90.f,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveDown = !bHit;
			}
			
			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 180.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveRight = !bHit;
			}

			{
				const FVector End = Start + FRotator(PlayerRotation.Pitch, 0.f, PlayerRotation.Roll).Vector() * 60.f;

				FHitResult HitResult;

				bool bHit = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					Start,
					End,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitResult,
					true
				);

				bCanMoveLeft = !bHit;
			}
		}
	}
}

