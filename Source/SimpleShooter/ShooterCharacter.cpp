// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
#include "Camera/CameraShakeBase.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainSpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));

	// ACharacter가 제공하는 함수로 인자 수정. Capsule -> GetCapsuleComponent()
	MainSpringArm->SetupAttachment(GetCapsuleComponent());
	MainCamera->SetupAttachment(MainSpringArm);

	CharacterStat = CreateDefaultSubobject<UShooterCharacterStat>(TEXT("CharacterStat"));
	
	GrenadeSpawnPoint  = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeSpawnPoint"));
	GrenadeSpawnPoint->SetupAttachment(GetCapsuleComponent());

	UltimateEffectPoint  = CreateDefaultSubobject<USceneComponent>(TEXT("UltimateEffectPoint"));
	UltimateEffectPoint->SetupAttachment(GetCapsuleComponent());

	HomingMissilePoint  = CreateDefaultSubobject<USceneComponent>(TEXT("HomingMissilePoint"));
	HomingMissilePoint->SetupAttachment(GetCapsuleComponent());
}
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
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
	StartBullet = Gun->OriginBullet;

	DefaultSockeOffset = MainSpringArm->SocketOffset;
	DefaultSpringArm = MainSpringArm->TargetArmLength;
	CurrentSpringArm = DefaultSpringArm;
	
}

bool AShooterCharacter::IsDead() const
{
	return HP <= 0;
}

float AShooterCharacter::HealthPercent() const
{
    return HP / MaxHP;
}

float AShooterCharacter::CoolTimePercent()
{
	if(bSaveSkillCool)
	{
		CurrentSkillCoolTime = CurrentSkillCoolTime - GetTimerTime();
	}
	else
	{
		CurrentSkillCoolTime = ReturnSkillCoolTime() - GetTimerTime();
	}
	
	float PercentTime = CurrentSkillCoolTime / ReturnSkillCoolTime();
    return PercentTime;
}

float AShooterCharacter::ReturnSkillCoolTime() const
{
    return SkillCoolTime;
}

float AShooterCharacter::ReturnDashCoolTime()
{
	float TotalCoolTime = DashCoolTime + DashDuration;
    return TotalCoolTime;
}

float AShooterCharacter::GetDashCoolTime()
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

int32 AShooterCharacter::GetCurrentBullet()
{
    return CurrentBullet;
}

int32 AShooterCharacter::GetFullBullet()
{
    return FullBullet;
}

float AShooterCharacter::ReturnGrenadeCoolTime()
{
    return GrenadeCoolTime;
}

float AShooterCharacter::GetGrenadeCoolTime()
{
	if(bSaveGrenadeCool)
	{
		CurrentGrenadeCoolTime = CurrentGrenadeCoolTime - GetGrenadeTimerTime();
	}
	else
	{
		CurrentGrenadeCoolTime = ReturnGrenadeCoolTime() - GetGrenadeTimerTime();
	}
	
	float PercentTime2 = CurrentGrenadeCoolTime / ReturnGrenadeCoolTime();
    return PercentTime2;
}

float AShooterCharacter::GetUltimateModeCoolTimePercent()
{
	if(bSaveUltimateCool)
	{
		CurrentUltimateCoolTime = CurrentUltimateCoolTime - GetUltimateTimerTime();
	}
	else
	{
		CurrentUltimateCoolTime = GetUltimateModeCoolTime() - GetUltimateTimerTime();
	}

	float PercentTime3 = CurrentUltimateCoolTime / GetUltimateModeCoolTime();
    return PercentTime3;
}

float AShooterCharacter::GetGhostModeCoolTimePercent()
{
	if(bSaveGhostCool)
	{
		CurrentGhostCoolTime = CurrentGhostCoolTime - GetGhostModeTimerTime();
	}
	else
	{
		CurrentGhostCoolTime = GetGhostModeCoolTime() - GetGhostModeTimerTime();
	}

	float PercentTime4 = CurrentGhostCoolTime / GetGhostModeCoolTime();
    return PercentTime4;
}

float AShooterCharacter::GetHomingMissileCoolTime()
{
    return HomingMissileCoolTime;
}

float AShooterCharacter::GetHomingMissileCoolTimePercent()
{
	if(bSaveHomingCool)
	{
		CurrentHomingCoolTime = CurrentHomingCoolTime - GetHomingMissileTimerTime();
	}
	else
	{
		CurrentHomingCoolTime = GetHomingMissileCoolTime() - GetHomingMissileTimerTime();
	}

	float PercentTime5 = CurrentHomingCoolTime / GetHomingMissileCoolTime();
    return PercentTime5;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bGhostMode)
	{
		CurrentSpringArm = FMath::FInterpTo(CurrentSpringArm, TargetSpringArm_Ghost, DeltaTime, GhostZoomSpeed);
		MainSpringArm->TargetArmLength = FMath::Lerp(MainSpringArm->TargetArmLength, CurrentSpringArm, AlphaMul);
		MainSpringArm->SocketOffset = TargetSocketOffset_Ghost;
	}
	else if(bUltimateReady)
	{
		if(!bUltimatMode)
		{
			CurrentSpringArm = FMath::FInterpTo(CurrentSpringArm, DefaultSpringArm + TargetSpringArm_Ultimate_Add, DeltaTime, GhostZoomSpeed);
			MainSpringArm->TargetArmLength = FMath::Lerp(MainSpringArm->TargetArmLength, CurrentSpringArm, AlphaMul);
			MainSpringArm->SocketOffset = TargetSocketOffset_Ultimate;
		}
		else
		{
			CurrentSpringArm = FMath::FInterpTo(CurrentSpringArm, DefaultSpringArm - TargetSpringArm_Ultimate_Add, DeltaTime, GhostZoomSpeed);
			MainSpringArm->TargetArmLength = FMath::Lerp(MainSpringArm->TargetArmLength, CurrentSpringArm, AlphaMul);
			MainSpringArm->SocketOffset = TargetSocketOffset_Ultimate;		
		}
	}

	else if(!bGhostMode && !bUltimateReady)
	{
		CurrentSpringArm = FMath::FInterpTo(CurrentSpringArm, DefaultSpringArm, DeltaTime, GhostZoomSpeed);
		MainSpringArm->TargetArmLength = FMath::Lerp(MainSpringArm->TargetArmLength, CurrentSpringArm, AlphaMul);
		MainSpringArm->SocketOffset = DefaultSockeOffset;
	}

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AShooterCharacter::OnShoot);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Released, this, &AShooterCharacter::StopShooting);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("EpicSkills"), EInputEvent::IE_Pressed, this, &AShooterCharacter::EpicSkills);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartJog);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Released, this, &AShooterCharacter::EndJog);
	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Pressed, this, &AShooterCharacter::GrenadeReady);
	PlayerInputComponent->BindAction(TEXT("UltimateMode"), EInputEvent::IE_Pressed, this, &AShooterCharacter::UltimateMode);
	PlayerInputComponent->BindAction(TEXT("GhostMode"), EInputEvent::IE_Pressed, this, &AShooterCharacter::GhostMode);
	PlayerInputComponent->BindAction(TEXT("HomingMissile"), EInputEvent::IE_Pressed, this, &AShooterCharacter::HomingMissileReady);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	if(bUltimating)
	{
		DamageApplied = DamageApplied / 2;
	}

	if(!bGodMode)
	{
		bGodMode = true;

		HP = HP - DamageApplied;
		UE_LOG(LogTemp, Display, TEXT("Your %f"), HP);
		SaveDataHP = CharacterStat->SetHP(HP);
		SetCanBeDamaged(false);
		bTakeDamage = true;
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AShooterCharacter::StopTakeDamage, TakeDamageTime, false);
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AShooterCharacter::CanBeDamagedInit, GodModeTime, false);
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(HitCameraShakeClass);
		if(!IsDead())
		{
			UGameplayStatics::SpawnSoundAttached(HitSound, GetMesh(), TEXT("HitSound"));
		}
	}
	

	if(IsDead())
	{
		UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
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

void AShooterCharacter::SetWeapon(AShotGun *NewWeapon)
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

int32 AShooterCharacter::GetCurrentGunNumbers()
{
    return CharacterStat->CurrentPlayerGun;
}

void AShooterCharacter::GetPortion(float PortionHP)
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

void AShooterCharacter::PlayerKnockBack(FVector KnockBackVec, bool bTakenStun, float TakenStunTime)
{
	if(!OnDash || !bNoneCC)
	{
		bKnockBack = true;
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter(KnockBackVec, true, true);
		bStunReady = bTakenStun;
		PlayerStunTime = TakenStunTime;
		CannotAllSkills();
		GetWorldTimerManager().SetTimer(KnockBackTimerHandle, this, &AShooterCharacter::ResetKnockBack, KnockBackTime, false);
	}
}

void AShooterCharacter::PlayerStun(float StunTime)
{
	if(!OnDash || !bNoneCC)
	{
		bStun = true;
		CannotAllSkills();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AShooterCharacter::ResetStun, StunTime, false);	
	}
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()*AxisValue);
	if(AxisValue < 0)
	{
		bCanFast = false;
	}
	else
	{
		bCanFast = true;
	}
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector()*AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	if(Skilling)
	{
		UE_LOG(LogTemp, Display, TEXT("Your message"));
		return;
	}
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Dash()
{	
	if(!OnDash && bCanDash && !bCoolDash)
	{
		bGodMode = true;
		bCoolDash = true;
		SetCanBeDamaged(false);
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		FVector DashVector = GetActorForwardVector()*DashSpeed;
		LaunchCharacter(DashVector, true, true);
		UGameplayStatics::SpawnSoundAttached(DashSound, GetMesh(), TEXT("DashSound"));
		UGameplayStatics::SpawnSoundAttached(DashSound_1, GetMesh(), TEXT("DashSound_1"));
		OnDash=true;
		CannotAllSkills();
		UGameplayStatics::SpawnEmitterAttached(DashEffect, GetMesh(), TEXT("Muzzle"));
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterCharacter::StopDash, DashDuration, false);
	}

}

void AShooterCharacter::StopDash()
{
	CanBeDamagedInit();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	CanAllSkills();
	bCanDash = false;
	OnDash=false;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterCharacter::ResetDash, DashCoolTime, false);
}

void AShooterCharacter::ResetDash()
{
	bCanDash = true;
	bCoolDash = false;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AShooterCharacter::CanBeDamagedInit()
{
	SetCanBeDamaged(true);
	bTakeDamage = false;
	bGodMode = false;
}

void AShooterCharacter::Shoot()
{
	if(bCanShot && OnAttackSpeed)
	{
		Gun->PullTrigger();
		
		if(Gun == nullptr) return;
		CurrentBullet = Gun->RemainBullet;
		InitAttackSpeed = Gun->AttackSpeedTime;
		GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &AShooterCharacter::ShootingAble, InitAttackSpeed, false);
	}
}

void AShooterCharacter::ShootingAble()
{
	Shoot();
}

void AShooterCharacter::EpicSkills()
{

	if(bCanPlaySkills && !GetCharacterMovement()->IsFalling() && !bCoolSkill)
	{
		Skilling = true;
		bGodMode = true;
		CannotAllSkills();
		bCoolSkill = true;
		SetCanBeDamaged(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		UGameplayStatics::SpawnSoundAttached(SkillSound, GetMesh(), TEXT("SkillSound"));
		GetWorldTimerManager().SetTimer(ReadyToSkillTimerHandle, this, &AShooterCharacter::PlaySkills, ReadyToSkills, false);
	}

}

bool AShooterCharacter::IsDash() const
{
    return OnDash;
}

bool AShooterCharacter::IsTakedDamage() const
{
    return bTakeDamage;
}

void AShooterCharacter::CanSkills()
{
	bCanPlaySkills = true;
	bCoolSkill = false;
	GetWorldTimerManager().ClearTimer(SkillTimerHandle);
}

float AShooterCharacter::GetTimerTime()
{
	float TimerTime = GetWorldTimerManager().GetTimerRate(SkillTimerHandle) - GetWorldTimerManager().GetTimerElapsed(SkillTimerHandle);
    return TimerTime;
}

void AShooterCharacter::PlaySkills()
{	
	UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(SkillCameraShakeClass);
	Skilling = false;
	Gun->OperatingSkiils();
	UGameplayStatics::SpawnSoundAttached(SkillSound_1, GetMesh(), TEXT("SkillSound_1"));
	CanAllSkills();
	bCanPlaySkills = false;
	CanBeDamagedInit();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &AShooterCharacter::CanSkills, SkillCoolTime, false);
}

float AShooterCharacter::GetDashTimerTime()
{
	float TimerTime = GetWorldTimerManager().GetTimerRate(TimerHandle) - GetWorldTimerManager().GetTimerElapsed(TimerHandle);
    return TimerTime;
}

float AShooterCharacter::GetUltimateModeCoolTime()
{
    return UltimateCoolTime;
}

float AShooterCharacter::GetGhostModeCoolTime()
{
    return GhostCoolTime;
}

void AShooterCharacter::SetReloadInint()
{
	Gun->RemainBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
	bRelodaing = false;
	CanAllSkills();
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void AShooterCharacter::InitGunSet(AShooterPlayerState *MyPlayerState)
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

void AShooterCharacter::SpawnGun(int32 GunNumber)
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

void AShooterCharacter::SetGunNumber()
{
	CharacterStat->CurrentPlayerGun = Gun->GunNumber;
}

void AShooterCharacter::StopTakeDamage()
{
	bTakeDamage = false;	
}

void AShooterCharacter::SetPlayerCoolTime(AShooterPlayerState *MyPlayerState)
{
	CurrentSkillCoolTime = MyPlayerState->PlayerSkillCoolTime_1;
	if(CurrentSkillCoolTime < ReturnSkillCoolTime())
	{
		CurrentSkillCoolTime = ReturnSkillCoolTime() - CurrentSkillCoolTime;
		if(CurrentSkillCoolTime < ReturnSkillCoolTime())
		{
			bSaveSkillCool = true;
			bCanPlaySkills = false;
			bCoolSkill = true;
			GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &AShooterCharacter::CanSkills, CurrentSkillCoolTime, false);
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
			bCoolDash  = true;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterCharacter::ResetDash, CurrentDashCoolTime, false);
			bSaveDashCool = false;
		}
	}

	CurrentUltimateCoolTime  = MyPlayerState->PlayerUltimateModeCoolTime;
	if(CurrentUltimateCoolTime < GetUltimateModeCoolTime())
	{
		CurrentUltimateCoolTime = GetUltimateModeCoolTime() - CurrentUltimateCoolTime;
		if(CurrentUltimateCoolTime < GetUltimateModeCoolTime())
		{
			bSaveUltimateCool = true;
			bCanUltimate = false;
			bCoolUltimate = true;
			GetWorldTimerManager().SetTimer(UltimateCoolTimerHandle, this, &AShooterCharacter::CoolDownUltimateMode, CurrentUltimateCoolTime, false);
			bSaveUltimateCool = false;
		}
	}

	CurrentGrenadeCoolTime = MyPlayerState->PlayerGrenadeCoolTime;
	if(CurrentGrenadeCoolTime < ReturnGrenadeCoolTime())
	{
		CurrentGrenadeCoolTime = ReturnGrenadeCoolTime() - CurrentGrenadeCoolTime;
		if(CurrentGrenadeCoolTime < ReturnGrenadeCoolTime())
		{
			bSaveGrenadeCool = true;
			bCanGrenade = false;
			bCoolGrenade = true;
			GetWorldTimerManager().SetTimer(ResetBombTimerHandle, this, &AShooterCharacter::ResetGrenadeCoolTime, CurrentGrenadeCoolTime, false);
			bSaveGrenadeCool = false;
		}
	}

	CurrentGhostCoolTime = MyPlayerState->PlayerGhostCoolTime;
	if(CurrentGhostCoolTime < GetGhostModeCoolTime())
	{
		CurrentGhostCoolTime = GetGhostModeCoolTime() - CurrentGhostCoolTime;
		if(CurrentGhostCoolTime < GetGhostModeCoolTime())
		{
			bSaveGhostCool = true;
			bCanGhostMode = false;
			bCoolGhostMode = true;
			GetWorldTimerManager().SetTimer(GhostTimerHandle, this, &AShooterCharacter::GhostCoolDown, CurrentGhostCoolTime, false);
			bSaveGhostCool = false;
		}
	}

	CurrentHomingCoolTime = MyPlayerState->PlayerHomingCoolTime;
	if(CurrentHomingCoolTime < GetHomingMissileCoolTime())
	{
		CurrentHomingCoolTime = GetHomingMissileCoolTime() - CurrentHomingCoolTime;
		if(CurrentHomingCoolTime < GetHomingMissileCoolTime())
		{
			bSaveHomingCool = true;
			bCanHomingMissile = false;
			bCoolHomingMissile = true;
			GetWorldTimerManager().SetTimer(HomingMissileTimerHandle, this, &AShooterCharacter::HomingMissileCoolDown, CurrentHomingCoolTime, false);
			bSaveHomingCool = false;
		}
	}
}

void AShooterCharacter::ResetKnockBack()
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

void AShooterCharacter::ResetStun()
{
	bStun = false;
	CanAllSkills();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
}

void AShooterCharacter::GrenadeReady()
{
	
	if(bCanGrenade && !bCoolGrenade)
	{
		bGrenadeAttack = true;
		bCoolGrenade = true;
		CannotAllSkills();
		GetWorldTimerManager().SetTimer(ReadyBombTimerHandle, this, &AShooterCharacter::ThrowGrenade, ReadyBombTime, false);
	}
}

void AShooterCharacter::ThrowGrenade()
{
	FVector GrenadeLocation = GrenadeSpawnPoint->GetComponentLocation();
	FRotator GrenadeRotation = GrenadeSpawnPoint->GetComponentRotation();

	AOilTrap* Grenade = GetWorld()->SpawnActor<AOilTrap>(GrenadeClass, GrenadeLocation, GrenadeRotation);

	Grenade->SetOwner(this);
	UGameplayStatics::SpawnSoundAttached(GrenadeSound, GetMesh(), TEXT("GrenadeSound"));
	GetWorldTimerManager().SetTimer(ReadyBombTimerHandle, this, &AShooterCharacter::ResetGrenade, ResetBombTime, false);
	GetWorldTimerManager().SetTimer(ResetBombTimerHandle, this, &AShooterCharacter::ResetGrenadeCoolTime, GrenadeCoolTime, false);
}

void AShooterCharacter::ResetGrenade()
{
	bGrenadeAttack = false;

	CanAllSkills();
	bCanGrenade = false;
	GetWorldTimerManager().ClearTimer(ReadyBombTimerHandle);
}

void AShooterCharacter::ResetGrenadeCoolTime()
{
	bCanGrenade = true;
	bCoolGrenade = false;
	GetWorldTimerManager().ClearTimer(ResetBombTimerHandle);
}

void AShooterCharacter::CannotAllSkills()
{
	bCanGrenade = false;
	bCanShot = false;
	bCanDash = false;
	bCanPlaySkills = false;
	bCanReload = false;
	bCanUltimate = false;
	bCanHomingMissile = false;
}

void AShooterCharacter::CanAllSkills()
{
	bCanShot = true;
	bCanDash = true;
	bCanPlaySkills = true;
	bCanReload = true;
	bCanGrenade = true;
	bCanUltimate = true;
	bCanHomingMissile = true;
}

void AShooterCharacter::UltimateMode()
{
	if(bCanUltimate && !bCoolUltimate)
	{
		CannotAllSkills();
		bCoolUltimate = true;
		bUltimateReady = true;
		bGodMode = true;
		bNoneCC = true;
		UGameplayStatics::SpawnSoundAttached(UltimateReadySound, GetMesh(), TEXT("UltimateReadySound"));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltModeEffect, UltimateEffectPoint->GetComponentLocation(), UltimateEffectPoint->GetComponentRotation(), UltEffectScale);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltModeReadyEffect, UltimateEffectPoint->GetComponentLocation(), UltimateEffectPoint->GetComponentRotation(), UltEffectScale);
		
		GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &AShooterCharacter::ChangeUltimateMode, UltimateTime, false);
		GetWorldTimerManager().SetTimer(UltimateCoolTimerHandle, this, &AShooterCharacter::CoolDownUltimateMode, UltimateCoolTime, false);
	}
}

void AShooterCharacter::ChangeUltimateMode()
{
	bUltimatMode = true;
	bUltimating =  true;
	UGameplayStatics::SpawnSoundAttached(UltimateSound, GetMesh(), TEXT("UltimateSound"));
	SettingUltimateMode();
	GetMesh()->SetSkinnedAssetAndUpdate(UltimateModeSkin);
	GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &AShooterCharacter::ResetUltmateMode, UltimateResetTime, false);
}

void AShooterCharacter::ResetUltmateMode()
{
	CanAllSkills();
	bUltimateReady = false;
	bCanUltimate = false;
	bUltimatMode = false;
	bNoneCC = false;
	bGodMode = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &AShooterCharacter::OriginalMode, UltimateMaintainTime, false);
}

void AShooterCharacter::OriginalMode()
{
	bUltimating =  false;
	ClearingUltimateMode();
	UGameplayStatics::SpawnEmitterAttached(ClearUltModeEffect, UltimateEffectPoint, TEXT("Clear"));
	GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &AShooterCharacter::SetOriginalSkin, SetOrinalModeTime, false);
}

void AShooterCharacter::SetOriginalSkin()
{
	GetMesh()->SetSkinnedAssetAndUpdate(OriginalModeSkin);
}

void AShooterCharacter::CoolDownUltimateMode()
{
	bCanUltimate = true;
	bCoolUltimate = false;
	GetWorldTimerManager().ClearTimer(UltimateCoolTimerHandle);
	GetWorldTimerManager().ClearTimer(UltimateTimerHandle);
}

float AShooterCharacter::GetUltimateTimerTime()
{
	float UltimateTimerTime = GetWorldTimerManager().GetTimerRate(UltimateCoolTimerHandle) - GetWorldTimerManager().GetTimerElapsed(UltimateCoolTimerHandle);
	 
    return UltimateTimerTime;
}

void AShooterCharacter::SettingUltimateMode()
{
	Gun->Damage = Gun->Damage * UltimateDamageUp;
	Gun->AttackSpeedTime = Gun->AttackSpeedTime / UltimateAttackSpeedUp;
	Gun->MaximumBullet = UltimateBulletNum;
	Gun->RemainBullet = Gun->MaximumBullet;
	FullBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
}

void AShooterCharacter::ClearingUltimateMode()
{
	Gun->Damage = Gun->Damage/ UltimateDamageUp;
	Gun->AttackSpeedTime = Gun->AttackSpeedTime * UltimateAttackSpeedUp;
	Gun->MaximumBullet = Gun->OriginBullet;
	Gun->RemainBullet = Gun->MaximumBullet;
	FullBullet = Gun->MaximumBullet;
	CurrentBullet = Gun->RemainBullet;
}

void AShooterCharacter::StartJog()
{
	if(!bRelodaing)
	{
		if(bCanFast)
		{
			GetCharacterMovement()->MaxWalkSpeed = 700.0f;	
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;	
		}
	}
}

void AShooterCharacter::EndJog()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AShooterCharacter::GhostMode()
{
	if(bCanGhostMode && !bCoolGhostMode)
	{
		UGameplayStatics::SpawnSoundAttached(GhostSound, GetMesh(), TEXT("GhostSound"));
		bGhostMode = true;
		bCanGhostMode = false;
		CannotAllSkills();
		bCoolGhostMode = true;
		bGodMode = true;
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
		for(AActor* Actor : Actors)
		{
			GetCapsuleComponent()->UPrimitiveComponent::IgnoreActorWhenMoving(Actor, true);
		}
		GhostModeUI();
		GetWorldTimerManager().SetTimer(ResetGhostTimerHandle, this, &AShooterCharacter::ResetGhostMode, GhostTime, false);
		GetWorldTimerManager().SetTimer(GhostTimerHandle, this, &AShooterCharacter::GhostCoolDown, GhostCoolTime, false);
	}

}

void AShooterCharacter::GhostCoolDown()
{
	bCanGhostMode = true;
	bCoolGhostMode = false;
	GetWorldTimerManager().ClearTimer(GhostTimerHandle);
}

void AShooterCharacter::ResetGhostMode()
{
	bGodMode = false;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
	for(AActor* Actor : Actors)
	{
		GetCapsuleComponent()->UPrimitiveComponent::IgnoreActorWhenMoving(Actor, false);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bGhostMode = false;
	CanAllSkills();
	bCanGhostMode = false;
	GhostModeUI();
}

float AShooterCharacter::GetGhostModeTimerTime()
{
	float GhostModeTimerTime = GetWorldTimerManager().GetTimerRate(GhostTimerHandle) - GetWorldTimerManager().GetTimerElapsed(GhostTimerHandle);
	 
    return GhostModeTimerTime;
}

void AShooterCharacter::GhostModeUI()
{
	ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(UGameplayStatics::GetPlayerPawn(this, 0)->GetController());
	if(PlayerController != nullptr)
	{
		if(bGhostMode)
		{
			PlayerController->SpawnGhostModeUI();
		}
		else
		{
			PlayerController->RemoveGhostModeUI();
		}
	}
}

void AShooterCharacter::HomingMissileReady()
{
	if(bCanHomingMissile && !bCoolHomingMissile)
	{
		UGameplayStatics::SpawnSoundAttached(MissileSound, GetMesh(), TEXT("MissileSound"));
		CannotAllSkills();
		bHomingMissile = true;
		bCanHomingMissile = false;
		bCoolHomingMissile = true;
		SpawnHomingMissile();
		GetWorldTimerManager().SetTimer(ResetHomingMissileTimerHandle, this, &AShooterCharacter::ResetHomingMissile, HomingMissileTime, false);
		GetWorldTimerManager().SetTimer(HomingMissileTimerHandle, this, &AShooterCharacter::HomingMissileCoolDown, HomingMissileCoolTime, false);
	}
}

void AShooterCharacter::SpawnHomingMissile()
{
	float MissileAngle =  FMath::RandRange(-MissileAngle_Max, MissileAngle_Max);
	FVector MissileLocation = HomingMissilePoint->GetComponentLocation();
	FRotator MissileRotation = HomingMissilePoint->GetComponentRotation()+ FRotator(MissileAngle, MissileAngle, 0);

	if(CurrentMissileNum == MissileNum)
	{
		CurrentMissileNum = 0;
		UE_LOG(LogTemp, Display, TEXT("Your fail"));
		return;
	}
	else
	{
		AAmmo* HomingMissile = GetWorld()->SpawnActor<AAmmo>(HomingMissileClass, MissileLocation, MissileRotation);
		HomingMissile->SetOwner(this);
		
		CurrentMissileNum = CurrentMissileNum + 1;
		UE_LOG(LogTemp, Display, TEXT("Your success"));
		GetWorldTimerManager().SetTimer(RepeatHomingMissileTimerHandle, this, &AShooterCharacter::SpawnHomingMissile, MissileRepeatTime, false);
	}
}

void AShooterCharacter::ResetHomingMissile()
{
	CanAllSkills();
	bCanHomingMissile = false;
	bHomingMissile = false;
}

void AShooterCharacter::HomingMissileCoolDown()
{
	bCanHomingMissile = true;
	bCoolHomingMissile = false;
	GetWorldTimerManager().ClearTimer(ResetHomingMissileTimerHandle);
	GetWorldTimerManager().ClearTimer(HomingMissileTimerHandle);
}

float AShooterCharacter::GetHomingMissileTimerTime()
{
	float HomingMissileTimerTime = GetWorldTimerManager().GetTimerRate(HomingMissileTimerHandle) - GetWorldTimerManager().GetTimerElapsed(HomingMissileTimerHandle);
	 
    return HomingMissileTimerTime;
}

float AShooterCharacter::GetGrenadeTimerTime()
{
	float GrenadeTimerTime = GetWorldTimerManager().GetTimerRate(ResetBombTimerHandle) - GetWorldTimerManager().GetTimerElapsed(ResetBombTimerHandle);
	 
    return GrenadeTimerTime;
}

bool AShooterCharacter::IsSkill()
{
    return Skilling;
}

bool AShooterCharacter::IsReloading()
{
    return bRelodaing;
}

bool AShooterCharacter::IsKnockBack()
{
    return bKnockBack;
}

bool AShooterCharacter::IsStun()
{
    return bStun;
}

bool AShooterCharacter::IsGrenadeAttack()
{
    return bGrenadeAttack;
}

bool AShooterCharacter::IsUltimateReady()
{
    return bUltimatMode;
}

bool AShooterCharacter::IsUltimating()
{
    return bUltimating;
}

bool AShooterCharacter::IsGhostMode()
{
    return bGhostMode;
}

bool AShooterCharacter::IsUltready()
{
    return bUltimateReady;
}

bool AShooterCharacter::IsHomingMissile()
{
    return bHomingMissile;
}

void AShooterCharacter::Reload()
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
	GetCharacterMovement()->MaxWalkSpeed = 320.0f;
	CannotAllSkills();
	UGameplayStatics::SpawnSoundAttached(ReloadSound, GetMesh(), TEXT("ReloadSound"));
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AShooterCharacter::SetReloadInint, ReloadDelay, false);
}

void AShooterCharacter::StopShooting()
{
	OnAttackSpeed = false;
}

void AShooterCharacter::OnShoot()
{
	OnAttackSpeed = true;
	Shoot();
}
