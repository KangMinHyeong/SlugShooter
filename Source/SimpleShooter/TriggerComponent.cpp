// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GunBox.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ShotGun.h"


UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        OnCharacterOverlap(Actor);
    }
    else
    {}

}

void UTriggerComponent::OnCharacterOverlap(AActor *OtherActor)
{
    AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
	    AShotGun* NewWeapon = GetWorld()->SpawnActor<AShotGun>(WeaponItemClass);
	    Character->SetWeapon(NewWeapon);
        UActorComponent::SetComponentTickEnabled(false);

		UGameplayStatics::SpawnEmitterAtLocation(this, Effect, GetComponentLocation(), GetComponentRotation());

    }
    else
    {
	    UE_LOG(LogTemp, Display, TEXT("no inchant"));
    }
}


AActor* UTriggerComponent::GetAcceptableActor() const
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
