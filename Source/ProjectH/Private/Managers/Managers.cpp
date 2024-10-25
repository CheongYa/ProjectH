// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/Managers.h"

void UManagers::InitGame() {
	Widget = World->SpawnActor<AWidgetManager>();
	
}

void UManagers::EndGame() {

}
