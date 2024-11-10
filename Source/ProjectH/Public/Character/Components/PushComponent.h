// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushComponent.generated.h"


class AMovable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPushComponent();

	void BeginPush(AMovable* Movable);
	void EndPush();
	
	bool IsHoldingObject() const;
	
	FVector GetDeltaLocation(float InDeltaSecond, float InForwardMove, float InRightMove);

	FORCEINLINE float GetPushRange() const { return PushRange; }

	FORCEINLINE void SetForwardMove(float InForwardMove) { ForwardMove = InForwardMove; }
	FORCEINLINE void SetRightMove(float InRightMove) { RightMove = InRightMove; }

	bool IsMovingAnObject() const { return bIsMovingAnObject; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	float PushRange;
	
	UPROPERTY()
	bool bIsMovingAnObject;

	UPROPERTY(EditAnywhere)
	bool bCanMoveRight;

	UPROPERTY(EditAnywhere)
	bool bCanMoveLeft;

	UPROPERTY(EditAnywhere)
	bool bCanMoveUp;

	UPROPERTY(EditAnywhere)
	bool bCanMoveDown;

	UPROPERTY()
	TObjectPtr<AMovable> MovableObject;

	UPROPERTY()
	float ForwardMove;

	UPROPERTY()
	float RightMove;

	UPROPERTY()
	float CurrentActorZRotation;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
