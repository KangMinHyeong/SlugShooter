// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameMonster.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGameMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayAttack();

	bool AttackTrace(FHitResult &HitResult, FVector &HitRotate);

	AController* OwnerController();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	bool bIsDead();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsTakedDamage() const;

	UFUNCTION(BlueprintPure)
	bool IsAttack() const;

	UFUNCTION(BlueprintPure)
	bool IsReady() const;

	UFUNCTION(BlueprintPure)
	bool IsStun() const;

	virtual void PossessedBy(AController* NewController) override;

	void Stun(float StunTime);

private:
	void CanBeDamagedInit();
	void StopAttackMotion();
	void Destoryed();
	void ResetReady();
	void ResetStun();

private:
	UPROPERTY(EditAnywhere)
	float MaxDistance = 200;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 50;

	UPROPERTY(EditAnywhere)
	float Damage = 50;

	UPROPERTY(EditAnywhere)
	float HP = 100;

	bool bTakeDamage = false;

	FTimerHandle HitTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle DeathTimerHandle;
	FTimerHandle ReadyTimerHandle;
	FTimerHandle StunTimerHandle;

	UPROPERTY(EditAnywhere)
	float bGodModeTime = 0.8f;

	UPROPERTY(EditAnywhere)
	float AttackMotionTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float ReadyMotionTime = 2.0f;

	UPROPERTY(EditAnywhere)
	float MonsterHigh = 40.0f;

	bool bDead = false;

	bool bAttack = false;

	bool bReadyMotion = false;

	UPROPERTY(EditAnywhere)
	float DeathLoadingTime = 2.0f;

	UPROPERTY(EditAnywhere)
	bool SpecialMonster = false;

	UPROPERTY(EditAnywhere)
	bool Grux = true;

protected:
	bool bStun = false;
	
	bool bGodMode = false;

	float DefaultVelocity = 0;

	FVector End = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* AttackEffect;

	UPROPERTY(EditAnywhere)
	FVector AttackEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	class USoundBase* DeadSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;
};
