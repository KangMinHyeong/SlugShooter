// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameMonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Ammo.h"

// Sets default values
ARangeMonsterCharacter::ARangeMonsterCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Ammo Spawning Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ARangeMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	bReady = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(ReadyTimerHandle, this, &ARangeMonsterCharacter::ClearReady, ReadyDelay, false);

	float SpeedRange = FMath::RandRange(410,520);
	GetCharacterMovement()->MaxWalkSpeed = SpeedRange;

	AGameMonsterAIController* MonsterAIController = Cast<AGameMonsterAIController>(GetController());
	if(MonsterAIController == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Your message"));
	}
	if(MonsterAIController != nullptr)
	{
		PossessedBy(MonsterAIController);
	}

	
}

// Called every frame
void ARangeMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARangeMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARangeMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);
	HP = HP - DamageApplied;

	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);

	//SetCanBeDamaged(false);
	bTakeDamage = true;
	if(!bGodMode)
	{
		//bGodMode = true;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ARangeMonsterCharacter::CanBeDamagedInit, bGodModeTime, false);
	}
	

	if(bIsDead())
	{
		UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
		bDead = true;
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ARangeMonsterCharacter::Destoryed, DeathLoadingTime, false);


	}

    return DamageApplied;
}

bool ARangeMonsterCharacter::bIsDead()
{
    return HP <= 0;
}

bool ARangeMonsterCharacter::IsDead() const
{
    return bDead;
}

bool ARangeMonsterCharacter::IsTakedDamage() const
{
    return bTakeDamage;
}

void ARangeMonsterCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
}

void ARangeMonsterCharacter::PlayProjectileAttack()
{
	if(bStun)
	{
		return;
	}
	FVector ProjectileLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileRotation = ProjectileSpawnPoint->GetComponentRotation();

	AAmmo* ProjectileActor = GetWorld()->SpawnActor<AAmmo>(AmmoClass, ProjectileLocation, ProjectileRotation);
    ProjectileActor->SetOwner(this);
	bProjectileAttack = true;
	
	UGameplayStatics::SpawnSoundAttached(AttackSound, GetMesh(), TEXT("AttackSound"));
	GetWorldTimerManager().SetTimer(ProjectileTimerHandle, this, &ARangeMonsterCharacter::ResetPose, AttackDelay, false);
}

bool ARangeMonsterCharacter::IsProjectileAttack()
{
    return bProjectileAttack;
}

bool ARangeMonsterCharacter::IsReady()
{
    return bReady;
}

bool ARangeMonsterCharacter::IsStun()
{
    return bStun;
}

void ARangeMonsterCharacter::CanBeDamagedInit()
{
	SetCanBeDamaged(true);
	bTakeDamage = false;
	bGodMode = false;
}

void ARangeMonsterCharacter::Destoryed()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}

void ARangeMonsterCharacter::ResetPose()
{
	bProjectileAttack = false;
	GetWorldTimerManager().ClearTimer(ProjectileTimerHandle);
}

void ARangeMonsterCharacter::ClearReady()
{
	bReady = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(ReadyTimerHandle);
}

void ARangeMonsterCharacter::Stun(float StunTime)
{
	bStun = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ARangeMonsterCharacter::ResetStun, StunTime, false);
}

void ARangeMonsterCharacter::ResetStun()
{
	bStun = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
}
