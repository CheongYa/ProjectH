// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MovableObject.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

AMovableObject::AMovableObject() {
	PType = PassingType::NonPassable;
}

void AMovableObject::BeginPlay() {
	Super::BeginPlay();
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AMovableObject::EndOverlap);
}

void AMovableObject::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("Contact Start")));
}

void AMovableObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AMovableObject::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("Contact End")));

	
}

