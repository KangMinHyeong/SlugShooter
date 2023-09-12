
// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAmmo.h"
#include "Kismet/GameplayStatics.h"


ASkillAmmo::ASkillAmmo()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ASkillAmmo::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ASkillAmmo::DestroyReady, SkillDuration, false);
    SetActorTickEnabled(false);
    Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASkillAmmo::ReturnHitMode, HitableTime, false);
}

void ASkillAmmo::ReturnHitMode()
{
    Projectile->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SetActorTickEnabled(true);
}


void ASkillAmmo::Tick(float DeltaTime)
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

void ASkillAmmo::OnCharacterOverlap(AActor *OtherActor)
{

    if (OtherActor != nullptr)
    {   
        SkillOnHit(OtherActor);
    }
    else
    {
    }
}

AActor *ASkillAmmo::GetAcceptableActor() const
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

void ASkillAmmo::SkillOnHit(AActor *OtherActor)
{
    auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());
		}

	}   
}

void ASkillAmmo::DestroySkillAmmo()
{
    Destroy();
}

void ASkillAmmo::DestroyReady()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, DestroyEffect, GetActorLocation(), GetActorRotation());
    GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ASkillAmmo::DestroySkillAmmo, DestroyDuration, false);
}