// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterSaveGame.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

UShooterSaveGame::UShooterSaveGame()
{
    HP = 1000;
    Location = FVector(-6688.000000, 1538.000000, 118.000000);
    ClearTags.Empty();
    GunNumber = 1;
    Bullet = 40;
	DashCool = 0;
    SkillCool_1 = 0;
    UltimateCool = 0;
    GrenadeCool = 0;
    GhostCool = 0;
    HomingCool = 0;
    bSaveOption = false;
    CurrentLevel = TEXT("Title");
}

void UShooterSaveGame::InitSaveData()
{
    auto MyPlayerState = GetMutableDefault<AShooterPlayerState>();
    if(MyPlayerState != nullptr)
    {
        HP = 1000;
    }
    Location = FVector(-6688.000000, 1538.000000, 118.000000);
    ClearTags.Empty();
    GunNumber = 1; 
    Bullet = 40;
    DashCool = 0;
    SkillCool_1 = 0;
    UltimateCool = 0;
    GrenadeCool = 0;
    GhostCool = 0;
    HomingCool = 0;
    bSaveOption = false;
    CurrentLevel = TEXT("Title");
}
