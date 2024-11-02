// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/WidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Data/WidgetAssets.h"

// Sets default values
AWidgetManager::AWidgetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UWidgetAssets> WidgetClassFinder(TEXT("/Script/ProjectH.WidgetAssets'/Game/Blueprints/Widgets/WidgetAssets.WidgetAssets'"));
	PopupWidgets = WidgetClassFinder.Object;
}

// Called when the game starts or when spawned
void AWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	// DisplayWidget = CreateWidget<UUserWidget>(GetWorld(), PopupWidgets->Get(FString(TEXT("Maze"))));
	// DisplayWidget->AddToViewport();
}

void AWidgetManager::PopupWidget(const FString& key) {
	if(DisplayWidget) {
		DisplayWidget->RemoveFromParent();
	}
	auto temp = PopupWidgets->Get(key);
	DisplayWidget = CreateWidget<UUserWidget>(GetWorld(), temp);
	DisplayWidget->AddToViewport();
}
