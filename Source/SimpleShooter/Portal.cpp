// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharacter.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PortalCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Portal Capsule"));
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
    Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));

	RootComponent = PortalCapsule;
    PortalMesh->SetupAttachment(PortalCapsule);
    Root->SetupAttachment(PortalCapsule);

	DestinationCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Destination Capsule"));
	DestinationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoDestinationrtal Mesh"));

	DestinationMesh->SetupAttachment(DestinationCapsule);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
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

void APortal::OnCharacterOverlap(AActor *OtherActor)
{
	Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
        GetWorld()->GetTimerManager().SetTimer(PortTimerHandle, this, &APortal::TelePort, 2);
    }
    else
    {
	    UE_LOG(LogTemp, Display, TEXT("no inchant"));
    }

}

void APortal::TelePort()
{
    Character->SetActorLocation(DestinationCapsule->GetComponentLocation());
    GetWorld()->GetTimerManager().ClearTimer(PortTimerHandle);   
    GetWorld()->GetTimerManager().SetTimer(PortTimerHandle, this, &APortal::TickEnable, 0.1f);
  
}

void APortal::TickEnable()
{
    AActor::SetActorTickEnabled(true);
    GetWorld()->GetTimerManager().ClearTimer(PortTimerHandle);
}

AActor *APortal::GetAcceptableActor() const
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

