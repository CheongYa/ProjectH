// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Managers/WidgetManager.h"
#include "Managers.generated.h"

class UWorld;

/**
 * 
 */
UCLASS()
class PROJECTH_API UManagers : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	inline void SetWorld(UWorld* world_) {
		World = world_;
	}

	UPROPERTY()
	TObjectPtr<AWidgetManager> Widget;


private:
	void InitGame();
	void EndGame();

	

	UPROPERTY()
	TObjectPtr<UWorld> World;
	
	friend class AGMB;
};
