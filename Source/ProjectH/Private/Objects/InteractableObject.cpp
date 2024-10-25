// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/InteractableObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Systems/GMB.h"
#include "Widgets/Popup/PopupBase.h"
#include "Components/PlayerStateComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/WidgetComp.h"

// Sets default values
AInteractableObject::AInteractableObject()
	: PType(PassingType::NonPassable)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	WidgetComp = CreateDefaultSubobject<UWidgetComp>(TEXT("Widget"));
	WidgetComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnOverlap);
	Capsule->OnComponentHit.AddDynamic(this, &AInteractableObject::OnHit);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::EndOverlap);

	switch(PType) {
	case PassingType::Passable:
	{
		Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));
		Capsule->SetCollisionProfileName(FName(TEXT("Passable")));
		break;
	}
	case PassingType::NonPassable:
	{
		Mesh->SetCollisionProfileName(FName(TEXT("BlockAll")));
		Capsule->SetCollisionProfileName(FName(TEXT("NonPassable")));
		break;
	}
	default:
		break;
	}
    
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObject::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Cast<APlayerCharacter>(OtherActor)->GetStateComponent()->SetPlayerState(EState::Contacting);
	Cast<APlayerCharacter>(OtherActor)->GetStateComponent()->AddContactObject(this);
	WidgetComp->SetVisible(true);
}

void AInteractableObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

}

void AInteractableObject::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Cast<APlayerCharacter>(OtherActor)->GetStateComponent()->SetPlayerState(EState::Idle);
	Cast<APlayerCharacter>(OtherActor)->GetStateComponent()->RemoveContactObject(this);
	WidgetComp->SetVisible(false);
}

