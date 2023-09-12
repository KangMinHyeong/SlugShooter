// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerState.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	void Shoot();
	void Reload();
	void StopShooting();
	void OnShoot();

	void EpicSkills();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float HealthPercent() const;

	UFUNCTION(BlueprintPure)
	float CoolTimePercent();

	UFUNCTION(BlueprintPure)
	float ReturnSkillCoolTime() const;

	UFUNCTION(BlueprintPure)
	float ReturnGrenadeCoolTime();

	UFUNCTION(BlueprintPure)
	float ReturnDashCoolTime();

	UFUNCTION(BlueprintPure)
	float GetDashCoolTime();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentBullet();

	UFUNCTION(BlueprintPure)
	int32 GetFullBullet();

	UFUNCTION(BlueprintPure)
	float GetGrenadeCoolTime();

	UFUNCTION(BlueprintPure)
	float GetGrenadeTimerTime();

	UFUNCTION(BlueprintPure)
	float GetDashTimerTime();

	UFUNCTION(BlueprintPure)
	float GetUltimateModeCoolTime();

	UFUNCTION(BlueprintPure)
	float GetGhostModeCoolTime();

	UFUNCTION(BlueprintPure)
	float GetUltimateModeCoolTimePercent();

	UFUNCTION(BlueprintPure)
	float GetGhostModeCoolTimePercent();

	UFUNCTION(BlueprintPure)
	float GetHomingMissileCoolTime();

	UFUNCTION(BlueprintPure)
	float GetHomingMissileCoolTimePercent();


	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SetWeapon(class AShotGun* NewWeapon);

	int32 GetCurrentGunNumbers();

	UPROPERTY()
	AGun* Gun;

	UFUNCTION(BlueprintPure)
	bool IsDash() const;

	UFUNCTION(BlueprintPure)
	bool IsTakedDamage() const;

	UFUNCTION(BlueprintPure)
	bool IsSkill();

	UFUNCTION(BlueprintPure)
	bool IsReloading();

	UFUNCTION(BlueprintPure)
	bool IsKnockBack();

	UFUNCTION(BlueprintPure)
	bool IsStun();

	UFUNCTION(BlueprintPure)
	bool IsGrenadeAttack();

	UFUNCTION(BlueprintPure)
	bool IsUltimateReady();

	UFUNCTION(BlueprintPure)
	bool IsUltimating();

	UFUNCTION(BlueprintPure)
	bool IsGhostMode();

	UFUNCTION(BlueprintPure)
	bool IsUltready();

	UFUNCTION(BlueprintPure)
	bool IsHomingMissile();
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* DashEffect;

	UPROPERTY(EditAnywhere)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere)
	float HP;

	void GetPortion(float PortionHP);

	void ShootingAble();

	float SaveDataHP;

	int32 FullBullet = 0;

	int32 CurrentBullet = 0;

	int32 StartBullet = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentGunNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSkillCoolTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentDashCoolTime = 0;

	UPROPERTY(EditAnywhere)
	float CurrentUltimateCoolTime = 0;

	UPROPERTY(EditAnywhere)
	float CurrentGrenadeCoolTime = 0;

	UPROPERTY(EditAnywhere)
	float CurrentHomingCoolTime = 0;

	UPROPERTY(EditAnywhere)
	float CurrentGhostCoolTime = 0;

	bool bSetPlayerLocation = false;

	void PlayerKnockBack(FVector KnockBackVec, bool bTakenStun, float TakenStunTime = 0.0f);

	void PlayerStun(float StunTime);

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void Dash();
	void StopDash();
	void ResetDash();
	void CanBeDamagedInit();
	void CanSkills();
	float GetTimerTime();
	void PlaySkills();
	void SetReloadInint();
	void InitGunSet(class AShooterPlayerState* MyPlayerState);
	void SpawnGun(int32 GunNumber);
	void SetGunNumber();
	void StopTakeDamage();
	void SetPlayerCoolTime(class AShooterPlayerState* MyPlayerState);
	void ResetKnockBack();
	void ResetStun();
	void ThrowGrenade();
	void GrenadeReady();
	void ResetGrenade();
	void ResetGrenadeCoolTime();
	void CannotAllSkills();
	void CanAllSkills();
	void UltimateMode();
	void ChangeUltimateMode();
	void ResetUltmateMode();
	void OriginalMode();
	void SetOriginalSkin();
	void CoolDownUltimateMode();
	float GetUltimateTimerTime();
	void SettingUltimateMode();
	void ClearingUltimateMode();
	void StartJog();
	void EndJog();
	void GhostMode();
	void GhostCoolDown();
	void ResetGhostMode();
	float GetGhostModeTimerTime();
	void GhostModeUI();
	void HomingMissileReady();
	void SpawnHomingMissile();
	void ResetHomingMissile();
	void HomingMissileCoolDown();
	float GetHomingMissileTimerTime();

private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* MainSpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere)
	float DashSpeed = 300;

	UPROPERTY(EditAnywhere)
	float ReadyToSkills = 1.0f;

	UPROPERTY(EditAnywhere)
	float DashDuration = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SkillCoolTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DashCoolTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float GodModeTime = 0.25f;

	UPROPERTY(EditAnywhere)
	float TakeDamageTime = 0.75f;

	UPROPERTY(EditAnywhere)
	float KnockBackTime = 1.2f;

	UPROPERTY(EditAnywhere)
	float ReadyBombTime = 0.25f;

	UPROPERTY(EditAnywhere)
	float ResetBombTime = 1.5f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShotGun> ShotGunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AOilTrap> GrenadeClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAmmo> HomingMissileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* GrenadeSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* UltimateEffectPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* HomingMissilePoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem* UltModeEffect;

	UPROPERTY(EditAnywhere)
	UParticleSystem* UltModeReadyEffect;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ClearUltModeEffect;

	bool OnDash = false;

	bool bCanShot = true;

	FTimerHandle TimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle SkillTimerHandle;
	FTimerHandle ReadyToSkillTimerHandle;
	FTimerHandle AttackSpeedTimerHandle;
	FTimerHandle ShootTimerHandle;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle DamageTimerHandle;
	FTimerHandle KnockBackTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle ReadyBombTimerHandle;
	FTimerHandle ResetBombTimerHandle;
	FTimerHandle UltimateTimerHandle;
	FTimerHandle UltimateCoolTimerHandle;
	FTimerHandle GhostTimerHandle;
	FTimerHandle ResetGhostTimerHandle;
	FTimerHandle HomingMissileTimerHandle;
	FTimerHandle ResetHomingMissileTimerHandle;
	FTimerHandle RepeatHomingMissileTimerHandle;

	bool bTakeDamage = false;

	bool bGodMode = false;

	bool bCanPlaySkills = true;

	bool Skilling = false;

	bool bCanDash = true;

	UPROPERTY(EditAnywhere)
	float ShootMotionDuration = 0.35f;

	UPROPERTY(EditAnywhere)
	float ReloadDelay = 2.1f;
	
	UPROPERTY(EditAnywhere)
	float GrenadeCoolTime = 6.0f;

	UPROPERTY(EditAnywhere)
	float UltimateTime = 1.5f;

	UPROPERTY(EditAnywhere)
	float UltimateCoolTime = 90.0f;

	UPROPERTY(EditAnywhere)
	float UltimateResetTime = 1.6f;

	UPROPERTY(EditAnywhere)
	float UltimateMaintainTime = 15.0f;

	UPROPERTY(EditAnywhere)
	float SetOrinalModeTime = 1.5f;

	UPROPERTY(EditAnywhere)
	FVector UltEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	float UltimateDamageUp = 1.25f;

	UPROPERTY(EditAnywhere)
	float UltimateAttackSpeedUp = 1.5f;

	UPROPERTY(EditAnywhere)
	int32 UltimateBulletNum = 999.0f;

	UPROPERTY(EditAnywhere)
	float GhostCoolTime = 30.0f;

	UPROPERTY(EditAnywhere)
	float GhostTime = 7.5f;

	UPROPERTY(EditAnywhere)
	float HomingMissileCoolTime = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float HomingMissileTime = 1.5f;

	UPROPERTY(EditAnywhere)
	float MissileAngle_Max = 15.0f;

	UPROPERTY(EditAnywhere)
	float MissileRepeatTime = 0.1f;

	UPROPERTY(EditAnywhere)
	int32 MissileNum = 3;

	float InitAttackSpeed;

	float PlayerStunTime = 0.0f;

	bool OnAttackSpeed = true;

	bool bRelodaing = false;

	class UShooterCharacterStat* CharacterStat;
	
	bool bSaveSkillCool = false;
	bool bSaveDashCool = false;
	bool bKnockBack = false;
	bool bStunReady = false;
	bool bStun = false;
	bool bCanReload = true;

	bool bGrenadeAttack = false;

	bool bCanGrenade = true;

	bool bSaveGrenadeCool = false;

	bool bUltimatMode = false;
	bool bCanUltimate = true;

	UPROPERTY(EditAnywhere)
	class USkinnedAsset * UltimateModeSkin;

	UPROPERTY(EditAnywhere)
	class USkinnedAsset * OriginalModeSkin;

	bool bNoneCC = false;

	bool bUltimating = false;

	bool bSaveUltimateCool = false;
	bool bSaveGhostCool = false;

	bool bGhostMode = false;
	bool bCanGhostMode = true;

	bool bCoolGhostMode = false;
	bool bCoolUltimate = false;
	bool bCoolGrenade = false;
	bool bCoolDash = false;
	bool bCoolSkill = false;
	bool bUltimateReady = false;

	FVector DefaultSockeOffset = FVector(0,0,0);
	float DefaultSpringArm = 0;
	float CurrentSpringArm = 0;

	bool bHomingMissile = false;
	bool bCanHomingMissile = true;
	bool bCoolHomingMissile = false;
	bool bSaveHomingCool = false;

	bool bCanFast = true;

	int32 CurrentMissileNum = 0;

	UPROPERTY(EditAnywhere)
	float TargetSpringArm_Ghost = 200.0f;

	UPROPERTY(EditAnywhere)
	float TargetSpringArm_Ultimate_Add = 500.0f;

	UPROPERTY(EditAnywhere)
	float GhostZoomSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
	float AlphaMul = 0.5f;

	UPROPERTY(EditAnywhere)
	FVector TargetSocketOffset_Ghost = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	FVector TargetSocketOffset_Ultimate = FVector(0,0,45);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> SkillCameraShakeClass;

	UPROPERTY(EditAnywhere)
	class USoundBase* DashSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DashSound_1;

	UPROPERTY(EditAnywhere)
	USoundBase* SkillSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SkillSound_1;

	UPROPERTY(EditAnywhere)
	USoundBase* GhostSound;

	UPROPERTY(EditAnywhere)
	USoundBase* MissileSound;

	UPROPERTY(EditAnywhere)
	USoundBase* UltimateSound;

	UPROPERTY(EditAnywhere)
	USoundBase* UltimateReadySound;

	UPROPERTY(EditAnywhere)
	USoundBase* GrenadeSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DeadSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;
};
