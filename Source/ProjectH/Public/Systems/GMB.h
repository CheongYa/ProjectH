// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMB.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AGMB : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
public:

	

private:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

