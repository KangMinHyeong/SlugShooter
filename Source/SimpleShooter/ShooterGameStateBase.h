// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AShooterGameStateBase();

public:
	void SaveClearStageWithTag(int32 StageTag);

	void SaveGame();

	void InitStateStat();

	void IsBattleMode(bool bBattle);

	void SaveLevel(FString LevelNames);

public:
	UPROPERTY(Transient)
	TArray<int32>ClearStageTags;

	UPROPERTY(Transient, BlueprintReadWrite)
	FName CurrentLevelName;

	FString SlotNameGS;

	bool bCanSave = true;
};
