// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMonster.h"
#include "MiddleBoss.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AMiddleBoss : public AGameMonster
{
	GENERATED_BODY()

public:
	AMiddleBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StunAttack();
	void ShieldMode();
	void VelocityUp();
	void CrushAttack();

	UFUNCTION(BlueprintPure)
	bool IsStunAttack();

	UFUNCTION(BlueprintPure)
	bool IsShieldMode();

	UFUNCTION(BlueprintPure)
	bool IsCrushAttack();

private:
	void StopStunAttackMotion();
	void ResetShieldMode();
	void ShieldAttack();
	void ResetShieldAttack();

private:
	FTimerHandle StunAttackTimerHandle;
	FTimerHandle ShieldModeTimerHandle;
	FTimerHandle CrushAttackTimerHandle;

	UPROPERTY(EditAnywhere)
	float StunAttackTime = 1.5f;

	UPROPERTY(EditAnywhere)
	float ShieldModeTime = 7.0f;

	UPROPERTY(EditAnywhere)
	float StunDamage = 70.0f;

	UPROPERTY(EditAnywhere)
	float GiveStunTime =3.5f;

	UPROPERTY(EditAnywhere)
	float CrushAttackTime = 0.2f;

	UPROPERTY(EditAnywhere)
	float ShieldAttackTime = 0.8f;

	UPROPERTY(EditAnywhere)
	float CrushDamage = 70.0f;

	UPROPERTY(EditAnywhere)
	float RushVelocity = 800.0f;

	UPROPERTY(EditAnywhere)
	float KnockBackDist = 500.0f;

	bool bStunAttack = false;

	bool bShieldMode = false;

	bool bCrushAttack = false;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* StunAttackEffect;

	UPROPERTY(EditAnywhere)
	FVector StunAttackEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ShieldAttackEffect;

	UPROPERTY(EditAnywhere)
	FVector ShieldAttackEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	USoundBase* ShieldModeSound;

	UPROPERTY(EditAnywhere)
	USoundBase* StunAttackSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CrushAttackSound;
};
