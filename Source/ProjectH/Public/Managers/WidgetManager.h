// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetManager.generated.h"

class UUserWidget;
class UWidgetAssets;

UCLASS()
class PROJECTH_API AWidgetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWidgetManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void PopupWidget();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetAssets> PopupWidgets;

	UPROPERTY()
	TSubclassOf<UUserWidget> CurrentWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> DisplayWidget;
};
