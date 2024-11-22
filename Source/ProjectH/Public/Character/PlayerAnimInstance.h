// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterAnimInstanceBase.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPlayerAnimInstance : public UCharacterAnimInstanceBase
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsHoldingAnObject;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMovingAnObject;
	
};
