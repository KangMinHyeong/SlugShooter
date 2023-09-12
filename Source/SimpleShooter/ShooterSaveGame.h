// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UShooterSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UShooterSaveGame();

	void InitSaveData();

public:
	UPROPERTY()
	float HP;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	int32 Bullet;

	UPROPERTY()
	TArray<int32>ClearTags;

	UPROPERTY()
	int32 GunNumber;

	UPROPERTY()
	float DashCool;

	UPROPERTY()
	float SkillCool_1;

	UPROPERTY()
	float UltimateCool;

	UPROPERTY()
	float GrenadeCool;

	UPROPERTY()
	float GhostCool;

	UPROPERTY()
	float HomingCool;

	UPROPERTY()
	bool bSaveOption;

	UPROPERTY()
	FName CurrentLevel;
};
