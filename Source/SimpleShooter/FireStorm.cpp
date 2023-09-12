// Fill out your copyright notice in the Description page of Project Settings.


#include "FireStorm.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFireStorm::AFireStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	FireTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FireTrigger"));
	FireTrigger->SetupAttachment(RootComp);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(FireTrigger);

	FireTrigger_None = CreateDefaultSubobject<UBoxComponent>(TEXT("FireTrigger_None"));
	FireTrigger_None->SetupAttachment(RootComp);

	FireEffect_Ready = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect_Ready"));
	FireEffect_Ready->SetupAttachment(FireTrigger_None);
}

// Called when the game starts or when spawned
void AFireStorm::BeginPlay()
{
	Super::BeginPlay();
	AActor::SetActorTickEnabled(false);

	FireTrigger->SetHiddenInGame(true, true);
	FireTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FireTrigger_None->SetHiddenInGame(true, true);
	FireTrigger_None->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(FireReadyTimerHandle, this, &AFireStorm::FireStormReady, FireReadyTime, false);
}

// Called every frame
void AFireStorm::Tick(float DeltaTime)
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

void AFireStorm::FireStormReady()
{
	FireTrigger_None->SetHiddenInGame(false, true);
	GetWorldTimerManager().SetTimer(FireReadyTimerHandle, this, &AFireStorm::FireStormPlay, FirePlayTime, true);
}

void AFireStorm::FireStormPlay()
{
	AActor::SetActorTickEnabled(true);
	FireTrigger_None->SetHiddenInGame(true, true);

	FireTrigger->SetHiddenInGame(false, true);
	FireTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UGameplayStatics::SpawnSoundAttached(ExplosionSound, RootComp, TEXT("ExplosionSound"));
	GetWorldTimerManager().SetTimer(FireReadyTimerHandle, this, &AFireStorm::FireReset, FireResetTime, true);
}

void AFireStorm::FireReset()
{
	UE_LOG(LogTemp, Display, TEXT("Your message"));
	FireTrigger->SetHiddenInGame(true, true);
	FireTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor::SetActorTickEnabled(false);
	GetWorldTimerManager().SetTimer(FireReadyTimerHandle, this, &AFireStorm::FireStormReady, FireReadyTime, false);
}

void AFireStorm::OnCharacterOverlap(AActor *OtherActor)
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
		FireOnHit(OtherActor);
    }
}

AActor *AFireStorm::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Character")))
        {
            return Actor;
        }
    }    

    return nullptr;
}

void AFireStorm::FireOnHit(AActor *OtherActor)
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

	}  
}
