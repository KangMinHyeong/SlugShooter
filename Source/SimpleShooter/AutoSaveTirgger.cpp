// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoSaveTirgger.h"
#include "SimpleShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

// Sets default values
AAutoSaveTirgger::AAutoSaveTirgger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SaveTrigger = CreateDefaultSubobject<UBoxComponent>("Save Trigger");
	RootComponent = SaveTrigger;
}

// Called when the game starts or when spawned
void AAutoSaveTirgger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoSaveTirgger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        OnCharacterOverlap(Actor);
    }
    else
    {}
}


void AAutoSaveTirgger::OnCharacterOverlap(AActor *OtherActor)
{
    AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
	    AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(Character->GetPlayerState());
    	if(MyPlayerState != nullptr)
    	{
			AActor::SetActorTickEnabled(false);
        	MyPlayerState->SetbSaveTrue();
            UE_LOG(LogTemp, Display, TEXT("Your save"));
    	}	

    }
    else
    {
	    UE_LOG(LogTemp, Display, TEXT("no inchant"));
    }
}

AActor* AAutoSaveTirgger::GetAcceptableActor() const
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