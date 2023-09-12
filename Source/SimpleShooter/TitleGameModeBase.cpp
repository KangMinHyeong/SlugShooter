// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameModeBase.h"
#include "ShooterPlayerState.h"
#include "ShooterGameStateBase.h"

ATitleGameModeBase::ATitleGameModeBase()
{
    PlayerStateClass = AShooterPlayerState::StaticClass();
    GameStateClass = AShooterGameStateBase::StaticClass();
}

void ATitleGameModeBase::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

	AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(NewPlayer->PlayerState);
	if(MyPlayerState == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your Idiot2"));
    }
    AShooterGameStateBase* MyGameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
	if(MyGameState == nullptr)
    {
        return;
    }
	MyPlayerState->InitPlayerStat();
    MyGameState->InitStateStat();
}
