// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameStateBase.h"
#include "ShooterSaveGame.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

AShooterGameStateBase::AShooterGameStateBase()
{
    ClearStageTags.Emplace(-2);

    SlotNameGS = TEXT("GameStateSlot");

    CurrentLevelName = TEXT("Title");
}

void AShooterGameStateBase::SaveClearStageWithTag(int32 StageTag)
{
    ClearStageTags.Emplace(StageTag);
}

void AShooterGameStateBase::SaveGame()
{
    UShooterSaveGame* SaveGameData = NewObject<UShooterSaveGame>();
    SaveGameData->ClearTags = ClearStageTags;
    SaveGameData->CurrentLevel = CurrentLevelName;

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, SlotNameGS, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Your Idiot"));
	}
}

void AShooterGameStateBase::InitStateStat()
{
    UShooterSaveGame* SaveGameData = Cast<UShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotNameGS, 0));
	if (nullptr == SaveGameData)
	{
		SaveGameData = GetMutableDefault<UShooterSaveGame>();
	}
    ClearStageTags = SaveGameData->ClearTags;
    CurrentLevelName = SaveGameData->CurrentLevel;
}

void AShooterGameStateBase::IsBattleMode(bool bBattle)
{
    bCanSave = !bBattle;
}

void AShooterGameStateBase::SaveLevel(FString MyLevelName)
{
    CurrentLevelName = FName(*MyLevelName);;

    SaveGame();
}
