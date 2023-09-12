// Fill out your copyright notice in the Description page of Project Settings.


#include "OilTrap.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "GameMonster.h"

// Sets default values
AOilTrap::AOilTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = Projectile;

	SmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Particle"));
	SmokeParticle->SetupAttachment(Projectile);

	TrapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Trap Component"));
	TrapComponent->SetupAttachment(Projectile);

	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Particle"));
	HitParticle->SetupAttachment(TrapComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AOilTrap::BeginPlay()
{
	Super::BeginPlay();

	if(bHomingGrenade)
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(Character != nullptr)
		{
			ProjectileMovementComponent->bIsHomingProjectile = true;
			ProjectileMovementComponent->HomingTargetComponent = Character->GetRootComponent();
			ProjectileMovementComponent->HomingAccelerationMagnitude = HomingSpeed;
		}
	}


	TrapComponent->SetHiddenInGame(true, true);
	TrapComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AOilTrap::HitAction, HitableTime, false);

	AActor::SetActorTickEnabled(false);
}

// Called every frame
void AOilTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {   
		if(bExplosionTrap)
		{
			AActor::SetActorTickEnabled(false);
		}
        OnCharacterOverlap(Actor);
    }
    else
    {}
}

void AOilTrap::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 


	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{	
		UE_LOG(LogTemp, Display, TEXT("Your Trap"));
		ProjectileMovementComponent->MaxSpeed = 10.0f;

		if(!bExplosionTrap)
		{
			Explosion();
		}
		else
		{
			float ExplosionTime = FMath::RandRange(ExplosionTime_Min, ExplosionTime_Max);
			GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AOilTrap::Explosion, ExplosionTime, false);
		}
	}
}

void AOilTrap::DestroyTrap()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AOilTrap::HitAction()
{
	Projectile->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Projectile->OnComponentHit.AddDynamic(this, &AOilTrap::OnHit);
}

AActor *AOilTrap::GetAcceptableActor() const
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

void AOilTrap::OnCharacterOverlap(AActor *OtherActor)
{

	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
	if(bEMP)
	{
		AGameMonster* Monster_1 = Cast<AGameMonster>(OtherActor);
		ARangeMonsterCharacter* Monster_2 = Cast<ARangeMonsterCharacter>(OtherActor);
		ABossMonster_Stage1* BossMonster = Cast<ABossMonster_Stage1>(OtherActor);
		if(Monster_1 != nullptr)
		{
			Monster_1->Stun(StunTime);
		}
		else if(Monster_2 != nullptr)
		{
			Monster_2->Stun(StunTime);
		}
		else if(BossMonster != nullptr)
		{
			BossMonster->Stun(StunTime);
		}
	}
}

void AOilTrap::FireStart()
{
	TrapComponent->SetHiddenInGame(false, true);
	TrapComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if(!bOilTrap)
	{
		TArray<AActor*> Actors;
		GetOverlappingActors(Actors);
		for (AActor* Actor : Actors)
		{
			if (Actor->ActorHasTag(AcceptableActorTag))
			{   
				OnCharacterOverlap(Actor);
			}
		}
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(BombCameraShakeClass);   
		UGameplayStatics::SpawnSoundAttached(ExplosionSound, Projectile, TEXT("ExplosionSound"));
	}

	if(bOilTrap)
	{
		AActor::SetActorTickEnabled(true);
		DestroyTime = FMath::RandRange(DestroyTime_Min, DestroyTime_Max);
		UGameplayStatics::SpawnSoundAttached(OilFireSound, Projectile, TEXT("OilFireSound"));
	}

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AOilTrap::DestroyTrap, DestroyTime, false);
}

void AOilTrap::Explosion()
{
	Projectile->SetEnableGravity(false);
	Projectile->SetHiddenInGame(true, true);
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, GetActorLocation(), GetActorRotation(), FireScale, true);

	if(bOilTrap)
	{
		UGameplayStatics::SpawnSoundAttached(OilSound, Projectile, TEXT("OilSound"));
	}
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AOilTrap::FireStart, FireTime, false);
}
