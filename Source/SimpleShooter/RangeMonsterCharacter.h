// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RangeMonsterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API ARangeMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARangeMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	bool bIsDead();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsTakedDamage() const;

	virtual void PossessedBy(AController* NewController) override;

	void PlayProjectileAttack();

	UFUNCTION(BlueprintPure)
	bool IsProjectileAttack();

	UFUNCTION(BlueprintPure)
	bool IsReady();

	UFUNCTION(BlueprintPure)
	bool IsStun();

	void Stun(float StunTime);

private:
	void CanBeDamagedInit();
	void Destoryed();
	void ResetPose();
	void ClearReady();
	void ResetStun();

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxDistance = 200;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HP = 100;

	bool bTakeDamage = false;

	bool bGodMode = false;

	FTimerHandle HitTimerHandle;
	FTimerHandle ProjectileTimerHandle;
	FTimerHandle DeathTimerHandle;
	FTimerHandle ReadyTimerHandle;
	FTimerHandle StunTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float bGodModeTime = 0.8f;

	bool bDead = false;

	bool bAttack = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLoadingTime = 2.0f;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AmmoScale = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AmmoSpeed = 750.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AAmmo> AmmoClass;

	bool bProjectileAttack = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDelay = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReadyDelay = 2.2f;

	bool bReady = false;

	bool bStun = false;

	UPROPERTY(EditAnywhere)
	class USoundBase* DeadSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;
};
