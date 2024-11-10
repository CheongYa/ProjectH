// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WidgetAssets.h"
#include "Managers/Managers.h"

TSubclassOf<UUserWidget> UWidgetAssets::Get(const FString& key) {
	return Widgets[key];

}
