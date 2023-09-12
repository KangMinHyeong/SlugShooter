// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotatingBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UBTTask_RotatingBoss::UBTTask_RotatingBoss()
{
    NodeName = TEXT("Rotating Boss");
}

EBTNodeResult::Type UBTTask_RotatingBoss::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ABossMonster_Stage1* BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
    if(BossMonster == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    auto Target = Cast<AShooterCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    if(Target == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your rotator null"));
        return EBTNodeResult::Failed;
    }

    FRotator MyTargetRotation = UKismetMathLibrary::FindLookAtRotation(BossMonster->GetActorLocation(), Target->GetActorLocation());

    BossMonster->SetRotating(MyTargetRotation);

    return EBTNodeResult::Succeeded;
}

