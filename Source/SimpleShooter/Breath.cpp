// Fill out your copyright notice in the Description page of Project Settings.


#include "Breath.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ABreath::ABreath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BreathRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BreathRoot"));
	RootComponent = BreathRoot;

	BreathTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Breath Trigger"));
	BreathTrigger->SetupAttachment(BreathRoot);

	BreathEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Breath Effect"));
	BreathEffect->SetupAttachment(BreathRoot);
}

// Called when the game starts or when spawned
void ABreath::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABreath::DestroyBreath, DestroyTime, false);

	GetWorld()->GetTimerManager().SetTimer(SoundTimerHandle, this, &ABreath::FireSoundLoop, SoundTime, true);

}


// Called every frame
void ABreath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BreathPlay();
}

void ABreath::OnCharacterOverlap(AActor *OtherActor)
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

    if (Character != nullptr)
    {
		BreathOnHit(OtherActor);
    }
}

AActor *ABreath::GetAcceptableActor() const
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

void ABreath::BreathPlay()
{
    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        OnCharacterOverlap(Actor);
    }
    else
    {}	
}

void ABreath::BreathOnHit(AActor *OtherActor)
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
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
		}

	}   
}

void ABreath::DestroyBreath()
{
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

void ABreath::FireSoundLoop()
{
	UGameplayStatics::SpawnSoundAttached(ExplosionSound, BreathRoot, TEXT("ExplosionSound"));
}
