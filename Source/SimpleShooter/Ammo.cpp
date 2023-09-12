// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "ShooterCharacter.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = Projectile;

	SmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Particle"));
	SmokeParticle->SetupAttachment(Projectile);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->MaxSpeed = 1300.0f;
	ProjectileMovementComponent->InitialSpeed = 1300.0f;
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	
	if(bFireBall)
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(Character != nullptr)
		{
			ProjectileMovementComponent->bIsHomingProjectile = true;
			ProjectileMovementComponent->HomingTargetComponent = Character->GetRootComponent();
			ProjectileMovementComponent->HomingAccelerationMagnitude = HomingSpeed;
		}
		GetWorldTimerManager().SetTimer(AmmoTimerHandle, this, &AAmmo::DestroyAmmo, AmmoDuration, false);
	}
	if(bHomingMissile)
	{
		TArray<AActor*> Actors;
    	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
		if(UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, MaxHomingDistance) != nullptr)
		{
			ProjectileMovementComponent->bIsHomingProjectile = true;
			ProjectileMovementComponent->HomingTargetComponent = UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, MaxHomingDistance)->GetRootComponent();
			ProjectileMovementComponent->HomingAccelerationMagnitude = HomingSpeed;
		}
	}

	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAmmo::HitAction, HitableTime, false);

}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if(TraceOn)
	//{
    	//FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLoc, DeltaTime, TraceSpeed);
    	//SetActorLocation(NewLocation);
	//}
	
}

void AAmmo::HitAction()
{
	Projectile->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Projectile->OnComponentHit.AddDynamic(this, &AAmmo::OnHit);
}

void AAmmo::SetTargeting(FVector TargetLocation)
{
	TargetLoc = TargetLocation;
}

void AAmmo::DestroyAmmo()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(AmmoTimerHandle);
}

void AAmmo::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
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
			UGameplayStatics::SpawnSoundAttached(ExplosionSound, Projectile, TEXT("ExplosionSound"));
		}

	}
	Destroy();
}
