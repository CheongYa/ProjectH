// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UPushComponent;
class AMovable;
class UInputAction;
struct FInputActionValue;
class UPlayerStateComponent;
/**
 * 
 */
UCLASS()
class PROJECTH_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	inline const TObjectPtr<UPlayerStateComponent> GetStateComponent() const {
		return State;
	}

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void EndPush();

	FORCEINLINE UPushComponent* GetPushComponent() const { return PushComponent; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	void Move(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Inventory(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlayerStateComponent> State;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPushComponent> PushComponent;

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	bool bIsOpenInventory;
};
