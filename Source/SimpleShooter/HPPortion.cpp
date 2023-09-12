// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPortion.h"
#include "Components/CapsuleComponent.h"
#include "ShooterCharacter.h"

// Sets default values
AHPPortion::AHPPortion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PorationCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Poration Capsule"));
	PorationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Poration Mesh"));

	RootComponent = PorationCapsule;
    PorationMesh->SetupAttachment(PorationCapsule);

}

// Called when the game starts or when spawned
void AHPPortion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPPortion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {   
        AActor::SetActorTickEnabled(false);
        OnCharacterOverlap(Actor);
    }
    else
    {}

}

void AHPPortion::OnCharacterOverlap(AActor *OtherActor)
{
	Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
        HPRecovery();
    }
    else
    {
    }

}

AActor *AHPPortion::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(AcceptableActorTag))
        {   
            return Actor;
        }
    }    
    return nullptr;
}

void AHPPortion::HPRecovery()
{
	Character->GetPortion(RecoveryHP);
    UE_LOG(LogTemp, Display, TEXT("Your message"));
}