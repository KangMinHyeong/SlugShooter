// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "SimpleShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameStateBase.h"
#include "ShooterSaveGame.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

	NewGameBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnNewGame")));
	if (NewGameBtn != nullptr)
	{
		NewGameBtn->OnClicked.AddDynamic(this, &UTitleWidget::NewGame);
	}
	ContinueBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnContinueGame")));
	if (ContinueBtn != nullptr)
	{
		ContinueBtn->OnClicked.AddDynamic(this, &UTitleWidget::Continue);
	}
}

void UTitleWidget::NewGame()
{	
    UE_LOG(LogTemp, Display, TEXT("Your NewGame"));
    UShooterSaveGame* SaveGameData = NewObject<UShooterSaveGame>();
	if(SaveGameData == nullptr) 
	{
		return;
	}
    
    SaveGameData->InitSaveData();

    auto MyPlayerState = GetDefault<AShooterPlayerState>();
	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, MyPlayerState->SaveSlotName, 0))
	{
		return;
	}
    auto MyGameState = GetDefault<AShooterGameStateBase>();
    if(MyGameState == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your gsnull"));
        return;
    }
    if (!UGameplayStatics::SaveGameToSlot(SaveGameData, MyGameState->SlotNameGS, 0))
	{
        UE_LOG(LogTemp, Display, TEXT("Your gs"));
		return;
	}
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("SlugShooter"));
    
}

void UTitleWidget::Continue()
{
	AShooterGameStateBase* MyGameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
	if(MyGameState != nullptr)
	{
		if(MyGameState->CurrentLevelName == TEXT("Title"))
		{
			UE_LOG(LogTemp, Display, TEXT("Your title"));
		}
		if(MyGameState->CurrentLevelName == TEXT("SlugShooter"))
		{
			UE_LOG(LogTemp, Display, TEXT("Your slug"));
		}
        UGameplayStatics::OpenLevel(this, MyGameState->CurrentLevelName);
    }	
}
