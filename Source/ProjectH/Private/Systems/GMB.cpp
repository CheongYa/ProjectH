// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/GMB.h"
#include "Managers/Managers.h"


void AGMB::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
	
}

void AGMB::BeginPlay() {
	Super::BeginPlay();
	UManagers* temp = GEngine->GetEngineSubsystem<UManagers>();
	temp->SetWorld(GetWorld());
	temp->InitGame();
}

void AGMB::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	GEngine->GetEngineSubsystem<UManagers>()->EndGame();
}
