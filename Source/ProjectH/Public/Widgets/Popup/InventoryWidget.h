// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Popup/PopupBase.h"
#include "InventoryWidget.generated.h"

class UCanvasPanel;
class UUniformGridPanel;

UCLASS()
class PROJECTH_API UInventoryWidget : public UPopupBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> Panel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;
};
