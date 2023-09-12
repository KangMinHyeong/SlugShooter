// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "ShooterGameStateBase.h"
#include "ShooterPlayerState.h"
#include "GameMonsterAIController.h"
#include "Kismet/GameplayStatics.h"

AKillEmAllGameMode::AKillEmAllGameMode()
{
    PlayerStateClass = AShooterPlayerState::StaticClass();
    GameStateClass = AShooterGameStateBase::StaticClass();
}

void AKillEmAllGameMode::PawnKilled(APawn *PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

    if(PlayerController != nullptr)
    {
        EndGame(false);
    }

    for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!AIController->IsDead()) return;
    }

    for(AGameMonsterAIController* MonsterAIController : TActorRange<AGameMonsterAIController>(GetWorld()))
    {
        if(!MonsterAIController->MonsterIsDead()) return;
		if(!MonsterAIController->RangeMonsterIsDead()) return;
    }
    

    EndGame(true);
}

void AKillEmAllGameMode::PostLogin(APlayerController *NewPlayer)
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

void AKillEmAllGameMode::StartPlay()
{
    Super::StartPlay();

}

AActor *AKillEmAllGameMode::ReturnBossAIController()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);
	for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Boss")))
        {   
            return Actor;
        }
    }    
    return nullptr;    
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerPawn)
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerPawn;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
    
}
