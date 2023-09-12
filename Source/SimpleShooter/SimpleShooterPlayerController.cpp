// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.h"
#include "ShooterPlayerState.h"
#include "Kismet/GameplayStatics.h"


void ASimpleShooterPlayerController::GameHasEnded(AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    
    if(bIsWinner)
    {

    }
    else
    {
        HUDWiget->RemoveFromParent();
        UUserWidget* LoseWidget = CreateWidget(this, LoseScreenWidget);
        if(LoseWidget != nullptr)
        {
            LoseWidget->AddToViewport();
            SetInputMode(UIInputMode);
            bShowMouseCursor = true;
        }
    }


}



void ASimpleShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUDWiget = CreateWidget(this, HUD);
    if(HUDWiget != nullptr)
    {
        HUDWiget->AddToViewport();

		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
    }

    ReloadWiget = CreateWidget(this, Reload);
    if(ReloadWiget != nullptr)
    {
        ReloadWiget->AddToViewport();
    }

}

void ASimpleShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ASimpleShooterPlayerController::OnGamePause);
}

void ASimpleShooterPlayerController::ChangeInputMode(bool bGameMode)
{
    if(bGameMode)
    {
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(UIInputMode);
        bShowMouseCursor = true;
    }
}

void ASimpleShooterPlayerController::RetryLevel()
{
    GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerController::RestartLevel, RespawnTime);
}

void ASimpleShooterPlayerController::CannotSave()
{
    CannotSaveWiget = CreateWidget(this, CantSave);
    if(CannotSaveWiget != nullptr)
    {
        CannotSaveWiget->AddToViewport(4);
        GetWorldTimerManager().SetTimer(SaveTimerHandle, this, &ASimpleShooterPlayerController::RemoverWidget, 1.0f);
    }
}

void ASimpleShooterPlayerController::RemoverWidget()
{
    CannotSaveWiget->RemoveFromParent();
}

void ASimpleShooterPlayerController::SpawnBossHPUI()
{
    BossHPWiget = CreateWidget(this, BossHP);
    if(BossHPWiget != nullptr)
    {
        BossHPWiget->AddToViewport();

    }
}

void ASimpleShooterPlayerController::RemoveBossHPUI()
{
    BossHPWiget->RemoveFromParent();
}

void ASimpleShooterPlayerController::SpawnGhostModeUI()
{
    GhostModeWidget = CreateWidget(this, GhostMode);
    if(GhostModeWidget != nullptr)
    {
        GhostModeWidget->AddToViewport();
    }
}

void ASimpleShooterPlayerController::RemoveGhostModeUI()
{
    GhostModeWidget->RemoveFromParent();
}

void ASimpleShooterPlayerController::ClearGameEnd()
{
    UUserWidget* WinWidget = CreateWidget(this, WinScreenWidget);
    if(WinWidget != nullptr)
    {
        WinWidget->AddToViewport();
    }
}

void ASimpleShooterPlayerController::OnGamePause()
{
    MenuWidget = CreateWidget<UGameplayWidget>(this, MenuWidgetClass);
    if(MenuWidget != nullptr)
    {
        MenuWidget->AddToViewport(3);

        SetPause(true);
        ChangeInputMode(false);
    }
}
