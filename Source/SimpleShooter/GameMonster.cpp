// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMonster.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameMonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterGameStateBase.h"
#include "SimpleShooterGameModeBase.h"


// Sets default values
AGameMonster::AGameMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AGameMonster::BeginPlay()
{
	Super::BeginPlay();

	bReadyMotion = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(ReadyTimerHandle, this, &AGameMonster::ResetReady, ReadyMotionTime, false);

	if(SpecialMonster)
	{
		float SpeedRange = FMath::RandRange(510.0f,600.0f);
		GetCharacterMovement()->MaxWalkSpeed = SpeedRange;

		float DamageRange =  FMath::RandRange(45, 60);
		Damage = DamageRange;
	}
	else
	{
		float SpeedRange = FMath::RandRange(410.0f,520.0f);
		GetCharacterMovement()->MaxWalkSpeed = SpeedRange;

		float DamageRange =  FMath::RandRange(25, 40);
		Damage = DamageRange;
	}

	AGameMonsterAIController* MonsterAIController = Cast<AGameMonsterAIController>(GetController());
	if(MonsterAIController == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Your message"));
	}
	if(MonsterAIController != nullptr)
	{
		PossessedBy(MonsterAIController);
	}

	if(Grux)
	{
		GetMesh()->GetBodyInstance(TEXT("weapon_r"))->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->GetBodyInstance(TEXT("weapon_l"))->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	DefaultVelocity = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void AGameMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameMonster::PlayAttack()
{
	if(!bStun)
	{
		FHitResult HitResult;
		FVector HitRotate;
		bool Line = AttackTrace(HitResult, HitRotate);

		if(Line)
		{
			if(AActor* Actor = HitResult.GetActor())
			{
				FPointDamageEvent DamageEvent(Damage, HitResult, HitRotate, nullptr);
				AController* MyController = Cast<AController>(GetController());
				if(MyController == nullptr) return;
				Actor->TakeDamage(Damage, DamageEvent, MyController, this);
			}
		}
		bAttack = true;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffect, End, FRotator::ZeroRotator, AttackEffectScale);
		UGameplayStatics::SpawnSoundAttached(AttackSound, GetMesh(), TEXT("AttackSound"));
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGameMonster::StopAttackMotion, AttackMotionTime, false);
	}
}

bool AGameMonster::AttackTrace(FHitResult &HitResult, FVector &HitRotate)
{
	FVector Location;
	FRotator Rotation;
	AController* MyController = Cast<AController>(GetController());
	if(MyController == nullptr)
	{
		return false;
	}
	MyController->GetPlayerViewPoint(Location, Rotation);
	End = GetActorLocation() + Rotation.Vector() * MaxDistance + FVector::UpVector* MonsterHigh;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitRotate = - Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, AttackRadius, 10, FColor::Green, false, 5);
	UGameplayStatics::SpawnSoundAttached(HitSound, GetMesh(), TEXT("HitSound"));

	FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), End, FQuat::Identity, ECC_GameTraceChannel4, Sphere, Params);
}

float AGameMonster::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	

	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);

	bTakeDamage = true;
	if(!bGodMode)
	{
		HP = HP - DamageApplied;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AGameMonster::CanBeDamagedInit, bGodModeTime, false);
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
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AGameMonster::Destoryed, DeathLoadingTime, false);

	}

    return DamageApplied;
}

bool AGameMonster::bIsDead()
{
	return HP <= 0;
}

bool AGameMonster::IsDead() const
{
    return bDead;
}

bool AGameMonster::IsTakedDamage() const
{
    return bTakeDamage;
}

bool AGameMonster::IsAttack() const
{
    return bAttack;
}

bool AGameMonster::IsReady() const
{
    return bReadyMotion;
}

bool AGameMonster::IsStun() const
{
    return bStun;
}

void AGameMonster::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
}

void AGameMonster::CanBeDamagedInit()
{
	SetCanBeDamaged(true);
	bTakeDamage = false;
	bGodMode = false;
}

void AGameMonster::StopAttackMotion()
{
	bAttack = false;
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

void AGameMonster::Destoryed()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}

void AGameMonster::ResetReady()
{
	bReadyMotion = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(ReadyTimerHandle);
}

void AGameMonster::Stun(float StunTime)
{
	bStun = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AGameMonster::ResetStun, StunTime, false);
}

void AGameMonster::ResetStun()
{
	bStun = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
}
