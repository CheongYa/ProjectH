// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/PushComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/Movable.h"
#include "Managers/Managers.h"
#include "Widgets/Popup/InventoryWidget.h"


APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(CameraBoom);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	State = CreateDefaultSubobject<UPlayerStateComponent>(TEXT("State"));
	bIsRunning = false;
	bIsOpenInventory = false;

	PushComponent = CreateDefaultSubobject<UPushComponent>(TEXT("PushComponent"));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndPush);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Inventory);
}

void APlayerCharacter::EndPush()
{
	if (IsValid(PushComponent))
	{
		PushComponent->EndPush();
	}
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector ForwardDirection = FVector(1, 0, 0); // X축 방향으로의 이동
	const FVector RightDirection = FVector(0, 1, 0); // Y축 방향으로의 이동

	if (!PushComponent->IsHoldingObject())
	{
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
	else
	{
		PushComponent->SetForwardMove(MovementVector.X);
		PushComponent->SetRightMove(MovementVector.Y);
	}

	if(State->GetPlayerState() == EState::Contacting)
		return;

	if(MovementVector == FVector2D::ZeroVector) {
		State->SetPlayerState(EState::Idle);
	}
	else {
		if(bIsRunning) {
			State->SetPlayerState(EState::Run);
		}
		else {
			State->SetPlayerState(EState::Walk);
		}
	}
}

void APlayerCharacter::Run(const FInputActionValue& Value)
{
	bIsRunning = Value.Get<bool>();
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? 600.f : 300.f;

}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3));

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			AMovable* Movable = Cast<AMovable>(HitActor);
			if (IsValid(Movable))
			{
				Movable->Hold(this);
			}
		}
	}

	// if(State->GetLastContactObject() != nullptr) {
	// 	State->GetLastContactObject()->Interact(this);
	// }
	// else {
	// 	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("No Objects")));
	// }
}

void APlayerCharacter::Inventory(const FInputActionValue& Value) {
	auto temp = Value.Get<bool>();
	if(temp && !bIsOpenInventory) {
		GEngine->GetEngineSubsystem<UManagers>()->Widget->PopupWidget(TEXT("Inventory"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
		bIsOpenInventory = true;
	} else {
		GEngine->GetEngineSubsystem<UManagers>()->Widget->RemovePopupWidget();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		bIsOpenInventory = false;
	}
}
