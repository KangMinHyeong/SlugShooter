// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPortal.h"
#include "AutoSaveTirgger.h"
#include "ShooterPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ALevelPortal::TelePort()
{
    AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(Character->GetPlayerState());
    if(MyPlayerState != nullptr)
    {
        MyPlayerState->SetPlayerHP(Character->HP);
        MyPlayerState->SetPlayerRemainBullet(Character->CurrentBullet);
		MyPlayerState->SetPlayerAttachedGun(Character->GetCurrentGunNumbers());
		MyPlayerState->SetPlayerCoolTime(Character->CurrentDashCoolTime, Character->CurrentSkillCoolTime, Character->CurrentUltimateCoolTime, Character->CurrentGrenadeCoolTime, Character->CurrentGhostCoolTime, Character->CurrentHomingCoolTime);
        MyPlayerState->SetbSaveFalse();
    }
    UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
    GetWorld()->GetTimerManager().ClearTimer(PortTimerHandle);   
    GetWorld()->GetTimerManager().SetTimer(PortTimerHandle, this, &APortal::TickEnable, 0.1f);
}
