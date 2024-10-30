// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"

#include "Character/CharacterBase.h"
#include "Character/PlayerCharacter.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
		if (IsValid(PlayerCharacter))
		{
			bIsHoldingAnObject = PlayerCharacter->IsHoldingObject();
			bIsMovingAnObject = PlayerCharacter->IsMovingAnObject();
		}
	}
}
