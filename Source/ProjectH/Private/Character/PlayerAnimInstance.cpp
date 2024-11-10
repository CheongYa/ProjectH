// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"

#include "Character/CharacterBase.h"
#include "Character/PlayerCharacter.h"
#include "Character/Components/PushComponent.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
		if (IsValid(PlayerCharacter))
		{
			UPushComponent* PushComponent = PlayerCharacter->GetPushComponent();
			if (PushComponent)
			{
				bIsHoldingAnObject = PushComponent->IsHoldingObject();
				bIsMovingAnObject = PushComponent->IsMovingAnObject();
			}
		}
	}
}
