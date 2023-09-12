// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSkill.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

AMonsterSkill::AMonsterSkill()
{
    PrimaryActorTick.bCanEverTick = true;

    SkillTrigger = CreateDefaultSubobject<UBoxComponent>("Skill Trigger");
    SkillTrigger->SetupAttachment(Projectile);

}

void AMonsterSkill::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &AMonsterSkill::DestroyReady, SkillDuration, false);
}

void AMonsterSkill::OnCharacterOverlap(AActor *OtherActor)
{
    if (OtherActor != nullptr)
    {   
        MonsterSkillOnHit(OtherActor);
    }
    else
    {
    }
}

void AMonsterSkill::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(UGameplayStatics::GetPlayerPawn(this, 0) != nullptr)
    {
        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation(), DeltaTime, Speed);
        SetActorLocation(NewLocation);
    }   

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        OnCharacterOverlap(Actor);
    }
    else
    {}
}

void AMonsterSkill::MonsterSkillOnHit(AActor *OtherActor)
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

void AMonsterSkill::DestroyReady()
{
    Destroy();
    GetWorldTimerManager().ClearTimer(SkillTimerHandle);
}

AActor *AMonsterSkill::GetAcceptableActor() const
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
