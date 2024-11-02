// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Popup/InventoryWidget.h"

void UInventoryWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("Hello")));
}
