// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WidgetAssets.generated.h"

class UWidgetBase;
class UUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTH_API UWidgetAssets : public UDataAsset
{
	GENERATED_BODY()
	
public:
	TSubclassOf<UUserWidget> Get(const FString& key);


protected:
	UPROPERTY(EditAnywhere)
	TMap<FString, TSubclassOf<UUserWidget>> Widgets;
};
