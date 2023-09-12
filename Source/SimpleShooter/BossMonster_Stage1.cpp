// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster_Stage1.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameMonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Breath.h"
#include "OilTrap.h"
#include "SimpleShooterGameModeBase.h"
#include "FireStorm.h"


// Sets default values
ABossMonster_Stage1::ABossMonster_Stage1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FireBallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FireBallSpawnPoint"));
	FireBallSpawnPoint->SetupAttachment(GetMesh());

	BreathSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BreathSpawnPoint"));
	BreathSpawnPoint->SetupAttachment(GetCapsuleComponent());

	SubMonsterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SubMonster SpawnPoint"));
	SubMonsterSpawnPoint->SetupAttachment(GetCapsuleComponent());

	SubRangeMonsterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SubRangeMonster SpawnPoint"));
	SubRangeMonsterSpawnPoint->SetupAttachment(GetCapsuleComponent());

	SubMonsterSpawnPoint_Middle = CreateDefaultSubobject<USceneComponent>(TEXT("SubMonster SpawnPoint_Middle"));
	SubMonsterSpawnPoint_Middle->SetupAttachment(GetCapsuleComponent());

	OilTrapSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("OilTrap SpawnPoint"));
	OilTrapSpawnPoint->SetupAttachment(GetMesh());

	ExplosionTrapSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionTrap SpawnPoint"));
	ExplosionTrapSpawnPoint->SetupAttachment(GetMesh());

	FireStormSpawnPoint_1 = CreateDefaultSubobject<USceneComponent>(TEXT("FireStormSpawnPoint_1"));
	FireStormSpawnPoint_1->SetupAttachment(GetCapsuleComponent());
	FireStormSpawnPoint_2 = CreateDefaultSubobject<USceneComponent>(TEXT("FireStormSpawnPoint_2"));
	FireStormSpawnPoint_2->SetupAttachment(GetCapsuleComponent());
	FireStormSpawnPoint_3 = CreateDefaultSubobject<USceneComponent>(TEXT("FireStormSpawnPoint_3"));
	FireStormSpawnPoint_3->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ABossMonster_Stage1::BeginPlay()
{
	Super::BeginPlay();
	HP = MaxHP;
	SpawnFireStorm();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bLevelStart = true;
	GetWorldTimerManager().SetTimer(LevelStartTimerHandle, this, &ABossMonster_Stage1::ResetLevelStart, LevelStartTime, false);		

	AGameMonsterAIController* MonsterAIController = Cast<AGameMonsterAIController>(GetController());
	if(MonsterAIController == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Your message"));
	}
	if(MonsterAIController != nullptr)
	{
		PossessedBy(MonsterAIController);
	}
	
	ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(UGameplayStatics::GetPlayerPawn(this, 0)->GetController());
	if(PlayerController != nullptr)
	{
		PlayerController->SpawnBossHPUI();
	}
	UGameplayStatics::SpawnSoundAttached(SpawnSound, GetMesh(), TEXT("SpawnSound"));
}

// Called every frame
void ABossMonster_Stage1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bRotation)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		FRotator RotatingMonster = FMath::RInterpConstantTo(GetActorRotation(), FocusTarget, DeltaTime, RotateVelo);
		SetActorRotation(RotatingMonster);
	}
}

// Called to bind functionality to input
void ABossMonster_Stage1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABossMonster_Stage1::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	SetCanBeDamaged(false);
	if(!bGodMode)
	{
		HP = HP - DamageApplied;
		UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);
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
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

		ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(UGameplayStatics::GetPlayerPawn(this, 0)->GetController());
		if(PlayerController != nullptr)
		{
			PlayerController->RemoveBossHPUI();
			PlayerController->ClearGameEnd();
		}

	}

    return DamageApplied;
}

void ABossMonster_Stage1::CanBeDamagedInit()
{
	SetCanBeDamaged(true);
	bGodMode = false;
}

bool ABossMonster_Stage1::bIsDead()
{
    return HP <= 0;
}

bool ABossMonster_Stage1::AttackTrace(FHitResult &HitResult, FVector &HitRotate)
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

	FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), End, FQuat::Identity, ECC_GameTraceChannel4, Sphere, Params);
}

bool ABossMonster_Stage1::RushAttackTrace(FHitResult &HitResult, FVector &HitRotate)
{
	FVector Location;
	FRotator Rotation;
	AController* MyController = Cast<AController>(GetController());
	if(MyController == nullptr)
	{
		return false;
	}
	MyController->GetPlayerViewPoint(Location, Rotation);
	End = GetActorLocation() + Rotation.Vector() * MaxRushDistance + FVector::UpVector* MonsterHigh;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitRotate = - Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, RushAttackRadius, 10, FColor::Green, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(RushAttackRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), End, FQuat::Identity, ECC_GameTraceChannel4, Sphere, Params);
}

void ABossMonster_Stage1::StopAttackMotion()
{
	bNormalAttack = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(NormalAttackTimerHandle);
}

void ABossMonster_Stage1::Destoryed()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}

void ABossMonster_Stage1::Rotating()
{
	bRotation = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(RotTimerHandle);
}

void ABossMonster_Stage1::ResetFireBallMotion()
{
	bFireBallMotion = false;
	GetWorldTimerManager().ClearTimer(FireBallTimerHandle);
}

void ABossMonster_Stage1::ResetBreathMotion()
{
	bBreathMotion= false;
	GetWorldTimerManager().ClearTimer(BreathTimerHandle);
}

void ABossMonster_Stage1::Breathing()
{
	bBreathing = true;
	GetWorldTimerManager().SetTimer(BreathingTimerHandle, this, &ABossMonster_Stage1::LoopBreath, StartBreathTime, false);
}

void ABossMonster_Stage1::SpawnBreath()
{	
	bBreathMotion= true;
	UGameplayStatics::SpawnSoundAttached(BreathAttackSound, GetMesh(), TEXT("BreathAttackSound"));
	GetWorldTimerManager().SetTimer(BreathTimerHandle, this, &ABossMonster_Stage1::ResetBreathMotion, BreathTime, false);
}

void ABossMonster_Stage1::ResetRushReady()
{
	bRushReady = false;
	GetWorldTimerManager().ClearTimer(RushReadyTimerHandle);
}

void ABossMonster_Stage1::ResetRushAttack()
{
	bRushAttack = false;
	Damage = Damage / 2;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(RushAttackTimerHandle);
}

void ABossMonster_Stage1::ResetExplosionReady()
{
	bExplosionTrapReady = false;
	GetWorldTimerManager().ClearTimer(ExplosionTrapReadyTimerHandle);
}

void ABossMonster_Stage1::ResetThrowOilTrap()
{
	bThrowOilTrap = false;
	GetWorldTimerManager().ClearTimer(ThrowOilTrapTimerHandle);
}

void ABossMonster_Stage1::ThrowFireBall()
{
	FVector FireBallLocation = FireBallSpawnPoint->GetComponentLocation();
	FRotator FireBallRotation = FireBallSpawnPoint->GetComponentRotation();
	AAmmo* FireBall = GetWorld()->SpawnActor<AAmmo>(FireBallClass, FireBallLocation, FireBallRotation);
	if(FireBall == nullptr) return;

    FireBall->SetOwner(this);
	UGameplayStatics::SpawnSoundAttached(FireBallSound, GetMesh(), TEXT("FireBallSound"));
	GetWorldTimerManager().SetTimer(FireBallTimerHandle, this, &ABossMonster_Stage1::ResetFireBallMotion, FireBallTime, false);	
}

void ABossMonster_Stage1::ResetThrowExplosionTrap()
{
	bThrowExplosionTrap = false;
	GetWorldTimerManager().ClearTimer(ThrowExplosionTrapTimerHandle);
}

void ABossMonster_Stage1::ResetLevelStart()
{
	bLevelStart = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(LevelStartTimerHandle);
}

void ABossMonster_Stage1::ResetBreathReady()
{
	bBreathReady = false;
	GetWorldTimerManager().ClearTimer(BreathReadyTimerHandle);
}

void ABossMonster_Stage1::EndBreathing()
{
	bBreathing = false;
	GetWorldTimerManager().ClearTimer(EndBreathTimerHandle);
}

void ABossMonster_Stage1::LoopBreath()
{
	FVector BreathLocation = BreathSpawnPoint->GetComponentLocation();
	FRotator BreathRotation = BreathSpawnPoint->GetComponentRotation();

	ABreath* Breath = GetWorld()->SpawnActor<ABreath>(BreathClass, BreathLocation, BreathRotation);
	if(Breath == nullptr) return;

    Breath->SetOwner(this);
	GetWorldTimerManager().SetTimer(EndBreathTimerHandle, this, &ABossMonster_Stage1::EndBreathing, EndBreathTime, false);
}

void ABossMonster_Stage1::SpawnFireBall(FVector TargetLocation)
{
	if(bStun)
	{
		return;
	}
	bFireBallMotion = true;
	GetWorldTimerManager().SetTimer(ThrowFireballimerHandle, this, &ABossMonster_Stage1::ThrowFireBall, 0.3, false);
}

void ABossMonster_Stage1::SpawnSubordinate()
{
	bSpawnSub = false;

	FVector SubMonsterSpawnLocation = SubMonsterSpawnPoint->GetComponentLocation();
	FRotator SubMonsterSpawnRotation = SubMonsterSpawnPoint->GetComponentRotation();

	for(int i = 0; i < MonsterNum; i++)
	{
		if(i%2 == 0)
		{
			AGameMonster* Monsters = GetWorld()->SpawnActor<AGameMonster>(SubMonsterClass, SubMonsterSpawnLocation - FVector::RightVector * SpawnDist * i/2, SubMonsterSpawnRotation);
		}
		else
		{
			AGameMonster* Monsters = GetWorld()->SpawnActor<AGameMonster>(SubMonsterClass, SubMonsterSpawnLocation + FVector::RightVector * SpawnDist * (i/2 + 1), SubMonsterSpawnRotation);			
		}
	}

	FVector SubRangeMonsterSpawnLocation = SubRangeMonsterSpawnPoint->GetComponentLocation();
	FRotator SubRangeMonsterSpawnRotation = SubRangeMonsterSpawnPoint->GetComponentRotation();

	for(int t = 0; t < RangeMonsterNum; t++)
	{
		if(t%2 == 0)
		{
			ARangeMonsterCharacter* RangeMonsters = GetWorld()->SpawnActor<ARangeMonsterCharacter>(SubRangeMonsterClass, SubRangeMonsterSpawnLocation - FVector::RightVector * SpawnDist * t/2, SubRangeMonsterSpawnRotation);
		}
		else
		{
			ARangeMonsterCharacter* RangeMonsters = GetWorld()->SpawnActor<ARangeMonsterCharacter>(SubRangeMonsterClass, SubRangeMonsterSpawnLocation + FVector::RightVector * SpawnDist * (t/2 + 1), SubRangeMonsterSpawnRotation);			
		}
	}
	FVector SubMiddleSpawnLocation = SubMonsterSpawnPoint_Middle->GetComponentLocation();
	FRotator SubMiddleSpawnRotation = SubMonsterSpawnPoint_Middle->GetComponentRotation();

	AMiddleBoss* MiddleBoss = GetWorld()->SpawnActor<AMiddleBoss>(MiddleBossClass, SubMiddleSpawnLocation, SubMiddleSpawnRotation);
	
	GetWorldTimerManager().ClearTimer(SpawnSubReadyTimerHandle);
}

void ABossMonster_Stage1::RushReady()
{
	bRushReady = true;
	UGameplayStatics::SpawnSoundAttached(RushAttackReadySound, GetMesh(), TEXT("RushAttackReadySound"));
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

	GetWorldTimerManager().SetTimer(RushReadyTimerHandle, this, &ABossMonster_Stage1::ResetRushReady, RushReadyTime, false);
}

void ABossMonster_Stage1::VelocityUp()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}

void ABossMonster_Stage1::SpawnSubReady()
{
	bSpawnSub = true;
	UGameplayStatics::SpawnSoundAttached(SpawnOrdinatekSound, GetMesh(), TEXT("SpawnOrdinatekSound"));
	GetWorldTimerManager().SetTimer(SpawnSubReadyTimerHandle, this, &ABossMonster_Stage1::SpawnSubordinate, SpawnSubTime, false);
}

void ABossMonster_Stage1::ExplosionTrapReady()
{
	if(bStun)
	{
		return;
	}
	bExplosionTrapReady = true;
	GetWorldTimerManager().SetTimer(ExplosionTrapReadyTimerHandle, this, &ABossMonster_Stage1::ResetExplosionReady, ExplosionTrapReadyTime, false);
}

void ABossMonster_Stage1::ThrowExplosionTrap()
{
	UGameplayStatics::SpawnSoundAttached(ExplosionTrapkSound, GetMesh(), TEXT("ExplosionTrapkSound"));
	bThrowExplosionTrap = true;
	FVector ExplosionTrapLocation = ExplosionTrapSpawnPoint->GetComponentLocation();
	FRotator ExplosionTrapRotation = ExplosionTrapSpawnPoint->GetComponentRotation();

	AOilTrap* Trap = GetWorld()->SpawnActor<AOilTrap>(ExplosionTrapClass, ExplosionTrapLocation, ExplosionTrapRotation);
	if(Trap == nullptr) return;

    Trap->SetOwner(this);
	
	GetWorldTimerManager().SetTimer(ThrowExplosionTrapTimerHandle, this, &ABossMonster_Stage1::ResetThrowExplosionTrap, ThrowExplosionTrapTime, false);
}

void ABossMonster_Stage1::ReadyOilTrap()
{
	if(bStun)
	{
		return;
	}
	bThrowOilTrap = true;
	GetWorldTimerManager().SetTimer(ReadyOilTrapTimerHandle, this, &ABossMonster_Stage1::ThrowOilTrap, ReadyOilTrapTimer, false);
}

void ABossMonster_Stage1::ThrowOilTrap()
{
	UGameplayStatics::SpawnSoundAttached(OilTrapkSound, GetMesh(), TEXT("OilTrapkSound"));
	FVector OilTrapLocation = OilTrapSpawnPoint->GetComponentLocation();
	FRotator OilTrapRotation = OilTrapSpawnPoint->GetComponentRotation();

	AOilTrap* Trap = GetWorld()->SpawnActor<AOilTrap>(OilTrapClass, OilTrapLocation, OilTrapRotation);
	if(Trap == nullptr) return;

    Trap->SetOwner(this);
	//Trap->SetTargeting(TargetLocation);
	GetWorldTimerManager().SetTimer(ThrowOilTrapTimerHandle, this, &ABossMonster_Stage1::ResetThrowOilTrap, ThrowOilTrapTime, false);
}

void ABossMonster_Stage1::EnHanceRush()
{	
	bEnhancedRush = true;
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
}

void ABossMonster_Stage1::RushAttack()
{
	bRushAttack = true;
	FHitResult HitResult;
	FVector HitRotate;
	Damage = Damage * 2;
	bool Line = RushAttackTrace(HitResult, HitRotate);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, HitRotate, nullptr);
			AController* MyController = Cast<AController>(GetController());
			if(MyController == nullptr) return;
			Actor->TakeDamage(Damage, DamageEvent, MyController, this);
			
			FVector KnockBackVec = GetActorForwardVector() * KnockBackDist;
			AShooterCharacter* Shooter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
			if(Shooter != nullptr)
			{	
				Shooter->PlayerKnockBack(KnockBackVec, bEnhancedRush, EnhancedRushStunTime);
				bEnhancedRush = false;
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RushAttackEffect, End, FRotator::ZeroRotator, RushAttackEffectScale);
	UGameplayStatics::SpawnSoundAttached(RushAttackSound, GetMesh(), TEXT("RushAttackSound"));
	GetWorldTimerManager().SetTimer(RushAttackTimerHandle, this, &ABossMonster_Stage1::ResetRushAttack, RushAttackTime, false);
}

void ABossMonster_Stage1::NormalAttack()
{
	if(bStun)
	{
		return;
	}
	FHitResult HitResult;
	FVector HitRotate;
	bool Line = AttackTrace(HitResult, HitRotate);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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
	bNormalAttack = true;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NormalAttackEffect, End, FRotator::ZeroRotator, NormalAttackEffectScale);
	UGameplayStatics::SpawnSoundAttached(NormalAttackSound, GetMesh(), TEXT("NormalAttackSound"));
	GetWorldTimerManager().SetTimer(NormalAttackTimerHandle, this, &ABossMonster_Stage1::StopAttackMotion, NormalAttackMotionTime, false);
}

void ABossMonster_Stage1::PlayBreath()
{
	bBreathReady = true;
	UGameplayStatics::SpawnSoundAttached(BreathReadySound, GetMesh(), TEXT("BreathReadySound"));
	GetWorldTimerManager().SetTimer(BreathReadyTimerHandle, this, &ABossMonster_Stage1::ResetBreathReady, BreathReadyTime, false);
}

float ABossMonster_Stage1::PercentBossHP() const
{
    return	HP/MaxHP;
}

bool ABossMonster_Stage1::IsDead() const
{
    return bDead;
}

bool ABossMonster_Stage1::IsNormalAttack() const
{
    return bNormalAttack;
}

bool ABossMonster_Stage1::IsRotation() const
{
    return bRotation;
}

bool ABossMonster_Stage1::IsFireBallAttack() const
{
    return bFireBallMotion;
}

bool ABossMonster_Stage1::IsBreathAttack() const
{
    return bBreathMotion;
}

bool ABossMonster_Stage1::IsBreathReady() const
{
    return bBreathReady;
}

bool ABossMonster_Stage1::IsRushReady() const
{
    return bRushReady;
}

bool ABossMonster_Stage1::IsRushAttack() const
{
    return bRushAttack;
}

bool ABossMonster_Stage1::IsSpawnSub() const
{
    return bSpawnSub;
}

bool ABossMonster_Stage1::IsExplosionTrapReady() const
{
    return bExplosionTrapReady;
}

bool ABossMonster_Stage1::IsExplosionTrapThrow() const
{
    return bThrowExplosionTrap;
}

bool ABossMonster_Stage1::IsThrowOilTrap() const
{
    return bThrowOilTrap;
}

bool ABossMonster_Stage1::IsLevelStart() const
{
    return bLevelStart;
}

bool ABossMonster_Stage1::IsBreating() const
{
    return bBreathing;
}

bool ABossMonster_Stage1::IsStun() const
{
    return bStun;
}

void ABossMonster_Stage1::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
}

void ABossMonster_Stage1::SetRotating(FRotator Rotation)
{
	bRotation = true;
	FocusTarget = Rotation;
	GetWorldTimerManager().SetTimer(RotTimerHandle, this, &ABossMonster_Stage1::Rotating, RotTime, false);
}

void ABossMonster_Stage1::Stun(float StunTime)
{
	if(bGodMode)
	{
		UE_LOG(LogTemp, Display, TEXT("Your god"));
		return;
	}
	bStun = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ABossMonster_Stage1::ResetStun, StunTime, false);
}

void ABossMonster_Stage1::ResetStun()
{
	bStun = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
}

void ABossMonster_Stage1::SpawnFireStorm()
{
	FVector FireStormLocation = FireStormSpawnPoint_1->GetComponentLocation();
	FRotator FireStormRotation = FireStormSpawnPoint_1->GetComponentRotation();

	AFireStorm* FireStorm = GetWorld()->SpawnActor<AFireStorm>(FireStormClass, FireStormLocation, FireStormRotation);
	FireStorm->SetOwner(this);

	FVector FireStormLocation_2 = FireStormSpawnPoint_2->GetComponentLocation();
	FRotator FireStormRotation_2 = FireStormSpawnPoint_2->GetComponentRotation();

	AFireStorm* FireStorm_2 = GetWorld()->SpawnActor<AFireStorm>(FireStormClass, FireStormLocation_2, FireStormRotation_2);
	FireStorm_2->SetOwner(this);

	FVector FireStormLocation_3 = FireStormSpawnPoint_3->GetComponentLocation();
	FRotator FireStormRotation_3 = FireStormSpawnPoint_3->GetComponentRotation();

	AFireStorm* FireStorm_3 = GetWorld()->SpawnActor<AFireStorm>(FireStormClass, FireStormLocation_3, FireStormRotation_3);
	FireStorm_3->SetOwner(this);

}
