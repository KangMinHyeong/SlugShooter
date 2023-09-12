// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class SIMPLESHOOTER_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

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

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SetWeapon(class AShotGun* NewWeapon);

	int32 GetCurrentGunNumbers();

	UPROPERTY()
	class AGun* Gun;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentGunNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSkillCoolTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentDashCoolTime = 0;

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
	TSubclassOf<class AGun> GunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShotGun> ShotGunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AOilTrap> GrenadeClass;

	UPROPERTY(EditAnywhere)
	class USceneComponent* GrenadeSpawnPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* UltModeEffectPoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem* UltModeEffect;

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

	bool bTakeDamage = false;

	bool bGotMode = false;

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
	float UltimateTime = 0.5f;

	UPROPERTY(EditAnywhere)
	FVector UltEffectScale = FVector(1,1,1);

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

};
