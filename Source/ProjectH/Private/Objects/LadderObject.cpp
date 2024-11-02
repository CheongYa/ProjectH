// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/LadderObject.h"

ALadderObject::ALadderObject() {
	PType = PassingType::NonPassable;
}

void ALadderObject::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("Hello")));
}

void ALadderObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ALadderObject::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, FString(TEXT("Bye")));


}
