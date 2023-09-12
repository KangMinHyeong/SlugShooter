// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "ShooterSaveGame.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"


AShooterPlayerState::AShooterPlayerState()
{
	PlayerHP = 1000;
	PlayerLocation = FVector(0, 100, 0);
	PlayerGunNumber = 1;
	PlayerDashCoolTime = 0;
	PlayerSkillCoolTime_1 = 0;
	PlayerUltimateModeCoolTime = 0;
	PlayerGrenadeCoolTime = 0;
	PlayerGhostCoolTime = 0;
	PlayerHomingCoolTime = 0;
	bSave = false;
    SaveSlotName = TEXT("Player1");
	
}

void AShooterPlayerState::SavePlayerStat()
{
    UShooterSaveGame* SaveGameData = NewObject<UShooterSaveGame>();
    SaveGameData->HP = PlayerHP;
	SaveGameData->Location = PlayerLocation;
	SaveGameData->Bullet = PlayerRemainBullet;
	SaveGameData->GunNumber = PlayerGunNumber;
	SaveGameData->DashCool = PlayerDashCoolTime;
	SaveGameData->SkillCool_1 = PlayerSkillCoolTime_1;
	SaveGameData->UltimateCool = PlayerUltimateModeCoolTime;
	SaveGameData->GrenadeCool = PlayerGrenadeCoolTime;
	SaveGameData->GhostCool = PlayerGhostCoolTime;
	SaveGameData->HomingCool = PlayerHomingCoolTime;
	SaveGameData->bSaveOption = bSave;

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Your Idiot"));
	}
}

void AShooterPlayerState::InitPlayerStat()
{
    UShooterSaveGame* SaveGameData = Cast<UShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == SaveGameData)
	{
		SaveGameData = GetMutableDefault<UShooterSaveGame>();
	}

    PlayerHP = SaveGameData->HP;
	PlayerLocation = SaveGameData->Location;
	PlayerRemainBullet = SaveGameData->Bullet;
	PlayerGunNumber = SaveGameData->GunNumber;
	PlayerDashCoolTime = SaveGameData->DashCool;
	PlayerSkillCoolTime_1 = SaveGameData->SkillCool_1;
	PlayerUltimateModeCoolTime = SaveGameData->UltimateCool;
	PlayerGrenadeCoolTime = SaveGameData->GrenadeCool;
	PlayerGhostCoolTime = SaveGameData->GhostCool;
	PlayerHomingCoolTime = SaveGameData->HomingCool;
	bSave = SaveGameData->bSaveOption;
}

void AShooterPlayerState::SetPlayerHP(float HP)
{
    PlayerHP = HP;

    UE_LOG(LogTemp, Display, TEXT("Remain HP : %f"), PlayerHP);

    SavePlayerStat();
}

void AShooterPlayerState::SetPlayerLocation(FVector Location)
{
	PlayerLocation = Location;
	bSave =  true;
	
	SavePlayerStat();
}

void AShooterPlayerState::SetPlayerRemainBullet(int32 RemainBullet)
{
	PlayerRemainBullet = RemainBullet;

	SavePlayerStat();
}

void AShooterPlayerState::SetPlayerAttachedGun(int32 Number)
{
	PlayerGunNumber = Number;

	SavePlayerStat();
}

void AShooterPlayerState::SetPlayerCoolTime(float Dash, float Skill_1, float Ultimate, float Grenade, float Ghost, float Homing)
{
	PlayerDashCoolTime = Dash;
	PlayerSkillCoolTime_1 = Skill_1;
	PlayerUltimateModeCoolTime = Ultimate;
	PlayerGrenadeCoolTime = Grenade;
	PlayerGhostCoolTime = Ghost;
	PlayerHomingCoolTime = Homing;

	SavePlayerStat();
}

void AShooterPlayerState::SetbSaveFalse()
{
	bSave =  false;
	SavePlayerStat();
}

void AShooterPlayerState::SetbSaveTrue()
{
	bSave =  true;
	SavePlayerStat();
}

void AShooterPlayerState::BeginPlay()
{
	Super::BeginPlay();

}
