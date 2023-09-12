// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossMonster_Stage1.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABossMonster_Stage1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossMonster_Stage1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SpawnFireBall(FVector TargetLocation);

	void SpawnSubordinate();

	void RushAttack();

	void NormalAttack();

	void PlayBreath();

	UFUNCTION(BlueprintPure)
	float PercentBossHP() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsNormalAttack() const;

	UFUNCTION(BlueprintPure)
	bool IsRotation() const;

	UFUNCTION(BlueprintPure)
	bool IsFireBallAttack() const;

	UFUNCTION(BlueprintPure)
	bool IsBreathAttack() const;

	UFUNCTION(BlueprintPure)
	bool IsBreathReady() const;	

	UFUNCTION(BlueprintPure)
	bool IsRushReady() const;	

	UFUNCTION(BlueprintPure)
	bool IsRushAttack() const;	

	UFUNCTION(BlueprintPure)
	bool IsSpawnSub() const;

	UFUNCTION(BlueprintPure)
	bool IsExplosionTrapReady() const;	

	UFUNCTION(BlueprintPure)
	bool IsExplosionTrapThrow() const;	

	UFUNCTION(BlueprintPure)
	bool IsThrowOilTrap() const;

	UFUNCTION(BlueprintPure)
	bool IsLevelStart() const;	

	UFUNCTION(BlueprintPure)
	bool IsBreating() const;

	UFUNCTION(BlueprintPure)
	bool IsStun() const;		

	virtual void PossessedBy(AController* NewController) override;

	void SetRotating(FRotator Rotation);

	void RushReady();

	void VelocityUp();

	void SpawnSubReady();

	void ExplosionTrapReady();
	void ThrowExplosionTrap();

	void ReadyOilTrap();
	void ThrowOilTrap();

	void EnHanceRush();

	void SpawnBreath();
	void Breathing();

	void Stun(float StunTime);

private:
	void CanBeDamagedInit();
	bool bIsDead();
	bool AttackTrace(FHitResult &HitResult, FVector &HitRotate);
	bool RushAttackTrace(FHitResult &HitResult, FVector &HitRotate);
	void StopAttackMotion();
	void Destoryed();
	void Rotating();
	void ResetFireBallMotion();
	void ResetBreathMotion();
	void ResetRushReady();
	void ResetRushAttack();
	void ResetExplosionReady();
	void ResetThrowOilTrap();
	void ThrowFireBall();
	void ResetThrowExplosionTrap();
	void ResetLevelStart();
	void ResetBreathReady();
	void EndBreathing();
	void LoopBreath();
	void ResetStun();
	void SpawnFireStorm();

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxDistance = 200;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRushDistance = 200;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RushAttackRadius = 50;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float HP = 100;

	UPROPERTY(EditAnywhere)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float bGodModeTime = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float NormalAttackMotionTime = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLoadingTime = 2.2f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RotTime = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireBallTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BreathTime = 3.0f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BreathReadyTime = 3.0f;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FireBallSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BreathSpawnPoint;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SubMonsterSpawnPoint;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SubMonsterSpawnPoint_Middle;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SubRangeMonsterSpawnPoint;

	UPROPERTY(EditAnywhere)
	class USceneComponent* ExplosionTrapSpawnPoint;

	UPROPERTY(EditAnywhere)
	class USceneComponent* OilTrapSpawnPoint;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FireStormSpawnPoint_1;
	UPROPERTY(EditAnywhere)
	class USceneComponent* FireStormSpawnPoint_2;
	UPROPERTY(EditAnywhere)
	class USceneComponent* FireStormSpawnPoint_3;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AAmmo> FireBallClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ABreath> BreathClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AGameMonster>SubMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ARangeMonsterCharacter>SubRangeMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AMiddleBoss>MiddleBossClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AOilTrap>OilTrapClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AOilTrap>ExplosionTrapClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AFireStorm>FireStormClass;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AmmoScale = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AmmoSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RushReadyTime = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RushAttackTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpawnSubTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ExplosionTrapReadyTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ThrowExplosionTrapTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ThrowOilTrapTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReadyOilTrapTimer = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LevelStartTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float EndBreathTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float StartBreathTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MonsterNum = 4;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 RangeMonsterNum = 2;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpawnDist = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float EnhancedRushStunTime = 5.0f;

	bool bFireBallMotion = false;

	FTimerHandle HitTimerHandle;
	FTimerHandle NormalAttackTimerHandle;
	FTimerHandle DeathTimerHandle;
	FTimerHandle RotTimerHandle;
	FTimerHandle FireBallTimerHandle;
	FTimerHandle BreathTimerHandle;
	FTimerHandle BreathReadyTimerHandle;
	FTimerHandle RushReadyTimerHandle;
	FTimerHandle RushAttackTimerHandle;
	FTimerHandle SpawnSubReadyTimerHandle;
	FTimerHandle ExplosionTrapReadyTimerHandle;
	FTimerHandle ReadyOilTrapTimerHandle;
	FTimerHandle ThrowOilTrapTimerHandle;
	FTimerHandle ThrowFireballimerHandle;
	FTimerHandle ThrowExplosionTrapTimerHandle;
	FTimerHandle LevelStartTimerHandle;
	FTimerHandle EndBreathTimerHandle;
	FTimerHandle BreathingTimerHandle;
	FTimerHandle StunTimerHandle;

	bool bDead = false;

	bool bNormalAttack = false;

	bool bRotation = false;
	bool bBreathMotion = false;

	FRotator FocusTarget;

	bool bRushReady = false;

	bool bRushAttack = false;

	bool bSpawnSub = false;

	bool bExplosionTrapReady = false;

	bool bThrowOilTrap = false;

	bool bThrowExplosionTrap = false;

	bool bLevelStart = false;

	bool bBreathing = false;

	bool bStun = false;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* NormalAttackEffect;

	UPROPERTY(EditAnywhere)
	FVector NormalAttackEffectScale = FVector(1,1,1);

	FVector End = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* RushAttackEffect;

	UPROPERTY(EditAnywhere)
	FVector RushAttackEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MonsterHigh = 1.0f;

	UPROPERTY(EditAnywhere)
	class USoundBase* DeadSound;

	UPROPERTY(EditAnywhere)
	USoundBase* NormalAttackSound;

	UPROPERTY(EditAnywhere)
	USoundBase* FireBallSound;

	UPROPERTY(EditAnywhere)
	USoundBase* RushAttackReadySound;

	UPROPERTY(EditAnywhere)
	USoundBase* RushAttackSound;

	UPROPERTY(EditAnywhere)
	USoundBase* BreathReadySound;

	UPROPERTY(EditAnywhere)
	USoundBase* BreathAttackSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnOrdinatekSound;

	UPROPERTY(EditAnywhere)
	USoundBase* OilTrapkSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionTrapkSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TargetRangeDist = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BreathRangeDist = 2200.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RotateVelo = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float KnockBackDist = 200.0f;

	UPROPERTY()
	bool bGodMode = false;

	UPROPERTY()
	bool bBreathReady = false;

	UPROPERTY()
	bool bEnhancedRush = false;

};
