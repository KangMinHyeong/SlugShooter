// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayWidget.h"
#include "Components/Button.h"
#include "SimpleShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "ShooterGameStateBase.h"
#include "ShooterPlayerState.h"

void UGameplayWidget::NativeConstruct()
{
    Super::NativeConstruct();

	ResumeBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeBtn)
	{
		ResumeBtn->OnClicked.AddDynamic(this, &UGameplayWidget::Resume);
	}

	ReturnToTitleBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleBtn)
	{
		ReturnToTitleBtn->OnClicked.AddDynamic(this, &UGameplayWidget::ReturnToTitle);
	}

	RetryBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryBtn)
	{
		RetryBtn->OnClicked.AddDynamic(this, &UGameplayWidget::Retry);
	}

	SaveBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnSaveData"))); 
		if (nullptr != SaveBtn)
	{
		SaveBtn->OnClicked.AddDynamic(this, &UGameplayWidget::Save);
	}

}

void UGameplayWidget::Resume()
{
    ASimpleShooterPlayerController* ResumeController = Cast<ASimpleShooterPlayerController>(GetOwningPlayer());
    if(ResumeController != nullptr)
    {
        RemoveFromParent();
        ResumeController->ChangeInputMode(true);
        ResumeController->SetPause(false);
    }

}

void UGameplayWidget::ReturnToTitle()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UGameplayWidget::Retry()
{
	ASimpleShooterPlayerController* RetryController = Cast<ASimpleShooterPlayerController>(GetOwningPlayer());
    if(RetryController != nullptr)
    {
		RetryController->RetryLevel();
    }

}

void UGameplayWidget::Save()
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(ShooterCharacter == nullptr) 
	{
		return;
	}
	
    AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(ShooterCharacter->GetController()->PlayerState);
    if(MyPlayerState == nullptr)
    {
        return;
    }
	AShooterGameStateBase* MyGameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
	if(MyGameState == nullptr)
    {
        return;
    }
	if(MyGameState->bCanSave)
	{
		MyPlayerState->SetPlayerHP(ShooterCharacter->HP);
		MyPlayerState->SetPlayerLocation(ShooterCharacter->GetActorLocation());
		if(ShooterCharacter->CurrentBullet > ShooterCharacter->StartBullet)
		{
			MyPlayerState->SetPlayerRemainBullet(ShooterCharacter->StartBullet);
		}
		else
		{
			MyPlayerState->SetPlayerRemainBullet(ShooterCharacter->CurrentBullet);
		}
		MyPlayerState->SetPlayerAttachedGun(ShooterCharacter->GetCurrentGunNumbers());
		MyPlayerState->SetPlayerCoolTime(ShooterCharacter->CurrentDashCoolTime, ShooterCharacter->CurrentSkillCoolTime, ShooterCharacter->CurrentUltimateCoolTime, ShooterCharacter->CurrentGrenadeCoolTime, ShooterCharacter->CurrentGhostCoolTime, ShooterCharacter->CurrentHomingCoolTime);
		MyGameState->SaveGame();
		MyGameState->SaveLevel(UGameplayStatics::GetCurrentLevelName(this));
	}
	else
	{
		ASimpleShooterPlayerController* SaveController = Cast<ASimpleShooterPlayerController>(GetOwningPlayer());
    	if(SaveController != nullptr)
    	{
        	SaveController->CannotSave();
    	}
	}
}
