// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WidgetComp.h"

UWidgetComp::UWidgetComp() {
	bDrawAtDesiredSize = false;
	SetPivot(FVector2D(1.0f, 1.0f));
	SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}