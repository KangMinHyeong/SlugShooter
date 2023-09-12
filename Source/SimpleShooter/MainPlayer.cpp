// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "TriggerComponent.h"
#include "ShotGun.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ShooterCharacterStat.h"
#include "ShooterPlayerState.h"
#include "Engine/SkinnedAsset.h"
#include "Components/SkinnedMeshComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStat = CreateDefaultSubobject<UShooterCharacterStat>(TEXT("CharacterStat"));
	
	GrenadeSpawnPoint  = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeSpawnPoint"));
	GrenadeSpawnPoint->SetupAttachment(GetMesh());

	UltModeEffectPoint  = CreateDefaultSubobject<USceneComponent>(TEXT("UltModeEffectPoint"));
	UltModeEffectPoint->SetupAttachment(GetMesh());
}
// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	SetGunNumber();

	AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(GetPlayerState());
	if(MyPlayerState != nullptr)
	{
		InitGunSet(MyPlayerState);
		HP = MyPlayerState->PlayerHP;
		SaveDataHP =HP;
		Gun->RemainBullet = MyPlayerState->PlayerRemainBullet;
		SetPlayerCoolTime(MyPlayerState);
		bSetPlayerLocation = MyPlayerState->bSave;
		if(bSetPlayerLocation)
		{
			SetActorLocation(MyPlayerState->PlayerLocation);
		}

	}
	
	FullBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
	
}

bool AMainPlayer::IsDead() const
{
	return HP <= 0;
}

float AMainPlayer::HealthPercent() const
{
    return HP / MaxHP;
}

float AMainPlayer::CoolTimePercent()
{
	if(bSaveSkillCool)
	{
		CurrentSkillCoolTime = CurrentSkillCoolTime - GetTimerTime();
	}
	else
	{
		CurrentSkillCoolTime = SkillCoolTime - GetTimerTime();
	}
	
	float PercentTime = CurrentSkillCoolTime / SkillCoolTime;
    return PercentTime;
}

float AMainPlayer::ReturnSkillCoolTime() const
{
    return SkillCoolTime;
}

float AMainPlayer::ReturnDashCoolTime()
{
	float TotalCoolTime = DashCoolTime + DashDuration;
    return TotalCoolTime;
}

float AMainPlayer::GetDashCoolTime()
{
	if(bSaveDashCool)
	{
		CurrentDashCoolTime = CurrentDashCoolTime - GetDashTimerTime();	
	}
	else
	{
		CurrentDashCoolTime = ReturnDashCoolTime() - GetDashTimerTime();
	}
	float PercentTime1 = CurrentDashCoolTime / ReturnDashCoolTime();
    return PercentTime1;
}

int32 AMainPlayer::GetCurrentBullet()
{
    return CurrentBullet;
}

int32 AMainPlayer::GetFullBullet()
{
    return FullBullet;
}

float AMainPlayer::ReturnGrenadeCoolTime()
{
    return GrenadeCoolTime;
}

float AMainPlayer::GetGrenadeCoolTime()
{
	float CurrentGrenadeCoolTime = ReturnGrenadeCoolTime() - GetGrenadeTimerTime();
	
	float PercentTime2 = CurrentGrenadeCoolTime / ReturnGrenadeCoolTime();
    return PercentTime2;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AMainPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AMainPlayer::OnShoot);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Released, this, &AMainPlayer::StopShooting);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AMainPlayer::Dash);
	PlayerInputComponent->BindAction(TEXT("EpicSkills"), EInputEvent::IE_Pressed, this, &AMainPlayer::EpicSkills);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AMainPlayer::Reload);
	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Pressed, this, &AMainPlayer::GrenadeReady);
	PlayerInputComponent->BindAction(TEXT("UltimateMode"), EInputEvent::IE_Pressed, this, &AMainPlayer::UltimateMode);
}

float AMainPlayer::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	if(!bGotMode)
	{
		bGotMode = true;

		HP = HP - DamageApplied;
		UE_LOG(LogTemp, Display, TEXT("Your %f"), HP);
		SaveDataHP = CharacterStat->SetHP(HP);
		SetCanBeDamaged(false);
		bTakeDamage = true;
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AMainPlayer::StopTakeDamage, TakeDamageTime, false);
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AMainPlayer::CanBeDamagedInit, GodModeTime, false);
	}
	

	if(IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

    return DamageApplied;
}

void AMainPlayer::SetWeapon(AShotGun *NewWeapon)
{
	if(NewWeapon == nullptr) return;

	Gun->Destroy();

	Gun = NewWeapon;
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	SetGunNumber();
	Gun->RemainBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
}

int32 AMainPlayer::GetCurrentGunNumbers()
{
    return CharacterStat->CurrentPlayerGun;
}

void AMainPlayer::GetPortion(float PortionHP)
{
	if(HP < MaxHP)
	{
		HP = HP + PortionHP;
		if(HP >= MaxHP)
		{
			HP = MaxHP;
		}	
	}

}

void AMainPlayer::PlayerKnockBack(FVector KnockBackVec, bool bTakenStun, float TakenStunTime)
{
	if(!OnDash)
	{
		bKnockBack = true;
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter(KnockBackVec, true, true);
		bStunReady = bTakenStun;
		PlayerStunTime = TakenStunTime;
		CannotAllSkills();
		GetWorldTimerManager().SetTimer(KnockBackTimerHandle, this, &AMainPlayer::ResetKnockBack, KnockBackTime, false);
	}
}

void AMainPlayer::PlayerStun(float StunTime)
{

	bStun = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AMainPlayer::ResetStun, StunTime, false);	
}

void AMainPlayer::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()*AxisValue);
}

void AMainPlayer::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector()*AxisValue);
}

void AMainPlayer::LookUp(float AxisValue)
{
	if(Skilling)
	{
		UE_LOG(LogTemp, Display, TEXT("Your message"));
		return;
	}
	AddControllerPitchInput(AxisValue);
}

void AMainPlayer::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMainPlayer::Dash()
{	
	if(!OnDash && bCanDash)
	{
		bGotMode = true;
		SetCanBeDamaged(false);
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		FVector DashVector = GetActorForwardVector()*DashSpeed;
		LaunchCharacter(DashVector, true, true);
		OnDash=true;
		CannotAllSkills();
		UGameplayStatics::SpawnEmitterAttached(DashEffect, GetMesh(), TEXT("Muzzle"));
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::StopDash, DashDuration, false);
	}

}

void AMainPlayer::StopDash()
{
	CanBeDamagedInit();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	CanAllSkills();
	bCanDash = false;
	OnDash=false;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::ResetDash, DashCoolTime, false);
}

void AMainPlayer::ResetDash()
{
	bCanDash = true;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AMainPlayer::CanBeDamagedInit()
{
	SetCanBeDamaged(true);
	bTakeDamage = false;
	bGotMode = false;
}

void AMainPlayer::Shoot()
{
	if(bCanShot && OnAttackSpeed)
	{
		Gun->PullTrigger();
		
		CurrentBullet = Gun->RemainBullet;
		InitAttackSpeed = Gun->AttackSpeedTime;
		GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &AMainPlayer::ShootingAble, InitAttackSpeed, false);
	}
}

void AMainPlayer::ShootingAble()
{
	Shoot();
}

void AMainPlayer::EpicSkills()
{

	if(bCanPlaySkills && !GetCharacterMovement()->IsFalling())
	{
		Skilling = true;
		bGotMode = true;
		CannotAllSkills();
		SetCanBeDamaged(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(ReadyToSkillTimerHandle, this, &AMainPlayer::PlaySkills, ReadyToSkills, false);
	}

}

bool AMainPlayer::IsDash() const
{
    return OnDash;
}

bool AMainPlayer::IsTakedDamage() const
{
    return bTakeDamage;
}

void AMainPlayer::CanSkills()
{
	bCanPlaySkills = true;
	GetWorldTimerManager().ClearTimer(SkillTimerHandle);
}

float AMainPlayer::GetTimerTime()
{
	float TimerTime = GetWorldTimerManager().GetTimerRate(SkillTimerHandle) - GetWorldTimerManager().GetTimerElapsed(SkillTimerHandle);
    return TimerTime;
}

void AMainPlayer::PlaySkills()
{	
	Skilling = false;
	Gun->OperatingSkiils();
	CanAllSkills();
	bCanPlaySkills = false;
	CanBeDamagedInit();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &AMainPlayer::CanSkills, SkillCoolTime, false);
}

float AMainPlayer::GetDashTimerTime()
{
	float TimerTime = GetWorldTimerManager().GetTimerRate(TimerHandle) - GetWorldTimerManager().GetTimerElapsed(TimerHandle);
    return TimerTime;
}

void AMainPlayer::SetReloadInint()
{
	Gun->RemainBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
	bRelodaing = false;
	CanAllSkills();
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void AMainPlayer::InitGunSet(AShooterPlayerState *MyPlayerState)
{
	int32 GunNumber = MyPlayerState->PlayerGunNumber;
	if(GunNumber == 1)
	{
		UE_LOG(LogTemp, Display, TEXT("Your aa"));
		return;
	}
	else
	{
		SpawnGun(GunNumber);
	}
	
}

void AMainPlayer::SpawnGun(int32 GunNumber)
{
	switch (GunNumber)
	{
	case 2:
	{
		Gun->Destroy();
		AShotGun* NewWeapon = GetWorld()->SpawnActor<AShotGun>(ShotGunClass);
		if(NewWeapon == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Your new"));
			return;
		}
		Gun->Destroy();
		Gun = NewWeapon;
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		SetGunNumber();
	}
	}
}

void AMainPlayer::SetGunNumber()
{
	CharacterStat->CurrentPlayerGun = Gun->GunNumber;
}

void AMainPlayer::StopTakeDamage()
{
	bTakeDamage = false;	
}

void AMainPlayer::SetPlayerCoolTime(AShooterPlayerState *MyPlayerState)
{
	CurrentSkillCoolTime = MyPlayerState->PlayerSkillCoolTime_1;
	if(SkillCoolTime > CurrentSkillCoolTime)
	{
		CurrentSkillCoolTime = SkillCoolTime - CurrentSkillCoolTime;
		if(CurrentSkillCoolTime < SkillCoolTime)
		{
			bSaveSkillCool = true;
			bCanPlaySkills = false;
			GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &AMainPlayer::CanSkills, CurrentSkillCoolTime, false);
			bSaveSkillCool = false;
		}
	}

	CurrentDashCoolTime = MyPlayerState->PlayerDashCoolTime;
	if(CurrentDashCoolTime < ReturnDashCoolTime())
	{
		CurrentDashCoolTime = ReturnDashCoolTime() - CurrentDashCoolTime;
		if(CurrentDashCoolTime < ReturnDashCoolTime())
		{
			bSaveDashCool = true;
			bCanDash = false;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::ResetDash, CurrentDashCoolTime, false);
			bSaveDashCool = false;
		}
	}

}

void AMainPlayer::ResetKnockBack()
{
	bKnockBack = false;
	if(bStunReady)
	{
		PlayerStun(PlayerStunTime);
	}
	else
	{
		CanAllSkills();
	}
	GetWorldTimerManager().ClearTimer(KnockBackTimerHandle);
}

void AMainPlayer::ResetStun()
{
	bStun = false;
	CanAllSkills();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
}

void AMainPlayer::GrenadeReady()
{
	
	if(bCanGrenade)
	{
		bGrenadeAttack = true;
		CannotAllSkills();
		GetWorldTimerManager().SetTimer(ReadyBombTimerHandle, this, &AMainPlayer::ThrowGrenade, ReadyBombTime, false);
	}
}

void AMainPlayer::ThrowGrenade()
{
	FVector GrenadeLocation = GrenadeSpawnPoint->GetComponentLocation();
	FRotator GrenadeRotation = GrenadeSpawnPoint->GetComponentRotation();

	AOilTrap* Grenade = GetWorld()->SpawnActor<AOilTrap>(GrenadeClass, GrenadeLocation, GrenadeRotation);

	Grenade->SetOwner(this);
	GetWorldTimerManager().SetTimer(ReadyBombTimerHandle, this, &AMainPlayer::ResetGrenade, ResetBombTime, false);
	GetWorldTimerManager().SetTimer(ResetBombTimerHandle, this, &AMainPlayer::ResetGrenadeCoolTime, GrenadeCoolTime, false);
}

void AMainPlayer::ResetGrenade()
{
	bGrenadeAttack = false;

	CanAllSkills();
	bCanGrenade = false;
	GetWorldTimerManager().ClearTimer(ReadyBombTimerHandle);
}

void AMainPlayer::ResetGrenadeCoolTime()
{
	bCanGrenade = true;
	GetWorldTimerManager().ClearTimer(ResetBombTimerHandle);
}

void AMainPlayer::CannotAllSkills()
{
	bCanGrenade = false;
	bCanShot = false;
	bCanDash = false;
	bCanPlaySkills = false;
	bCanReload = false;
}

void AMainPlayer::CanAllSkills()
{
	bCanShot = true;
	bCanDash = true;
	bCanPlaySkills = true;
	bCanReload = true;
	bCanGrenade = true;
}

void AMainPlayer::UltimateMode()
{
	if(bCanUltimate)
	{
		bUltimatMode = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &AMainPlayer::ChangeUltimateMode, UltimateTime, false);
	}
}

void AMainPlayer::ChangeUltimateMode()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltModeEffect, UltModeEffectPoint->GetComponentLocation(), UltModeEffectPoint->GetComponentRotation(), UltEffectScale);
	GetMesh()->SetSkinnedAssetAndUpdate(UltimateModeSkin);
}

float AMainPlayer::GetGrenadeTimerTime()
{
	float GrenadeTimerTime = GetWorldTimerManager().GetTimerRate(ResetBombTimerHandle) - GetWorldTimerManager().GetTimerElapsed(ResetBombTimerHandle);
	 
    return GrenadeTimerTime;
}

bool AMainPlayer::IsSkill()
{
    return Skilling;
}

bool AMainPlayer::IsReloading()
{
    return bRelodaing;
}

bool AMainPlayer::IsKnockBack()
{
    return bKnockBack;
}

bool AMainPlayer::IsStun()
{
    return bStun;
}

bool AMainPlayer::IsGrenadeAttack()
{
    return bGrenadeAttack;
}

void AMainPlayer::Reload()
{
	if(!bCanReload)
	{
		return;
	}
	if(CurrentBullet == FullBullet)
	{
		return;
	}
	bRelodaing = true;
	CannotAllSkills();
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AMainPlayer::SetReloadInint, ReloadDelay, false);
}

void AMainPlayer::StopShooting()
{
	OnAttackSpeed = false;
}

void AMainPlayer::OnShoot()
{
	OnAttackSpeed = true;
	Shoot();
}
