// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Movable.generated.h"

UCLASS()
class PROJECTH_API AMovable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovable();
	
	virtual void OnConstruction(const FTransform& Transform) override;

	void Hold(class APlayerCharacter* Character);

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh.Get(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	TArray<FTransform> PushTransforms;

	int32 FindClosestPushTransformIndex(const FVector2D& CharacterLocation, float PushRange);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
