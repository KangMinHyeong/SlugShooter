// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	AShooterPlayerState();

	void SavePlayerStat();

	void InitPlayerStat();

	void SetPlayerHP(float HP);

	void SetPlayerLocation(FVector Location);

	void SetPlayerRemainBullet(int32 RemainBullet);

	void SetPlayerAttachedGun(int32 Number);

	void SetPlayerCoolTime(float Dash, float Skill_1, float Ultimate, float Grenade, float Ghost, float Homing);

	void SetbSaveFalse();
	void SetbSaveTrue();

public:
	UPROPERTY(Transient)
	float PlayerHP;

	UPROPERTY(Transient)
	FVector PlayerLocation;

	UPROPERTY(Transient)
	int32 PlayerRemainBullet;

	UPROPERTY(Transient)
	int32 PlayerGunNumber;

	UPROPERTY(Transient)
	float PlayerSkillCoolTime_1;	

	UPROPERTY(Transient)
	float PlayerDashCoolTime;

	UPROPERTY(Transient)
	float PlayerUltimateModeCoolTime;

	UPROPERTY(Transient)
	float PlayerGrenadeCoolTime;

	UPROPERTY(Transient)
	float PlayerGhostCoolTime;

	UPROPERTY(Transient)
	float PlayerHomingCoolTime;	

	UPROPERTY(Transient)
	bool bSave = false;	

	FString SaveSlotName;
	
};
