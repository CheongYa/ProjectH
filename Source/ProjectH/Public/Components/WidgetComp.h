// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetComp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()
	UWidgetComp();

public:
	FORCEINLINE void SetVisible(const bool& flag) {
		bDrawAtDesiredSize = flag;
	}
};
