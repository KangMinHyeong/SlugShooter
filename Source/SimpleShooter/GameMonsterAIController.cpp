// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RangeMonsterCharacter.h"
#include "GameMonster.h"

void AGameMonsterAIController::BeginPlay()
{
    Super::BeginPlay();


}

bool AGameMonsterAIController::MonsterIsDead() const
{
    AGameMonster* Monster = Cast<AGameMonster>(GetPawn());
    if(Monster != nullptr)
    {
        return Monster->bIsDead();
    }
    return true;
}

bool AGameMonsterAIController::RangeMonsterIsDead() const
{
    ARangeMonsterCharacter* Monster = Cast<ARangeMonsterCharacter>(GetPawn());
    if(Monster != nullptr)
    {
        return Monster->bIsDead();
    }
    return true;
}

bool AGameMonsterAIController::MiddleBossIsDead() const
{
    AMiddleBoss* Monster = Cast<AMiddleBoss>(GetPawn());
    if(Monster != nullptr)
    {
        return Monster->bIsDead();
    }
    return true;
}

void AGameMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGameMonsterAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    if(AIBehavior != nullptr)
    {   
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        GetBlackboardComponent()->SetValueAsBool(TEXT("BreathCondition"), true);
    }
}
