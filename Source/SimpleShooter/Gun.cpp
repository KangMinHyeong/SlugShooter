// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraShakeBase.h"
#include "ShooterCharacter.h"
#include "Ammo.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));	
	GunMesh->SetupAttachment(Root);

	BulletSpawningPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Ammo Spawning Point"));
	BulletSpawningPoint->SetupAttachment(GunMesh);
}

void AGun::PullTrigger()
{
	if(RemainBullet > 0)
	{
		UGameplayStatics::SpawnEmitterAttached(ShootParticle, GunMesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(ShootSound, GunMesh, TEXT("MuzzleFlashSocket"));

		auto OwnerCharacter = Cast<AShooterCharacter>(GetOwner());
		if(OwnerCharacter != nullptr)
		{
			float ShootRecoil = FMath::RandRange(-0.15f, -ShootRecoil_Max);
			OwnerCharacter->AddControllerPitchInput(ShootRecoil);

			float ShootRecoil_Yaw = FMath::RandRange(ShootRecoil_Max_Yaw, -ShootRecoil_Max_Yaw);
			OwnerCharacter->AddControllerYawInput(ShootRecoil_Yaw);
		}

		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(ShootCameraShakeClass);
	
		FHitResult HitResult;
		FVector ParticleRotate;
		bool Line = GunTrace(HitResult, ParticleRotate);

		if(Line)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, ParticleRotate.Rotation());
		
			if(AActor* Actor = HitResult.GetActor())
			{
				FPointDamageEvent DamageEvent(Damage, HitResult, ParticleRotate, nullptr);
				AController* Controller = OwnerController();
				if(Controller == nullptr) return;
				Actor->TakeDamage(Damage, DamageEvent, Controller, this);

			}
		}
		RemainBullet = RemainBullet - 1;
		UE_LOG(LogTemp, Display, TEXT("Your %d"), RemainBullet);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Your %d"), RemainBullet);
	}

}

void AGun::OperatingSkiils()
{	
	FVector BulletLocation = BulletSpawningPoint->GetComponentLocation();
	FRotator BulletRotation = BulletSpawningPoint->GetComponentRotation();

	if(!ExplosiveAttack)
	{
		AAmmo* BulleteActor = GetWorld()->SpawnActor<AAmmo>(AmmoClass, BulletLocation, BulletRotation);
		BulleteActor->SetOwner(GetOwner());
		BulleteActor->SetActorScale3D(FVector(AmmoScale, AmmoScale, AmmoScale));
		BulleteActor->ProjectileMovementComponent->MaxSpeed = AmmoSpeed;
		BulleteActor->ProjectileMovementComponent->InitialSpeed = AmmoSpeed;
	}
	
	if(ExplosiveAttack)
	{
		AOilTrap* BulleteActor = GetWorld()->SpawnActor<AOilTrap>(ExplosionAmmoClass, BulletLocation, BulletRotation);
		BulleteActor->SetOwner(GetOwner());
		BulleteActor->SetActorScale3D(FVector(AmmoScale, AmmoScale, AmmoScale));
		BulleteActor->ProjectileMovementComponent->MaxSpeed = AmmoSpeed;
		BulleteActor->ProjectileMovementComponent->InitialSpeed = AmmoSpeed;	
	}

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	RemainBullet = MaximumBullet;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &HitResult, FVector &ParticleRotate)
{
	FVector Location;
	FRotator Rotation;

	AController* Controller = OwnerController();
	if(Controller == nullptr)
	{
		return false;
	}
	Controller->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	ParticleRotate = - Rotation.Vector();
    return GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::OwnerController()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn == nullptr) return nullptr;

    return Pawn->GetController();
}

