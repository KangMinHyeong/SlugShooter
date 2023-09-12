// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FireBall.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FireBall::UBTTask_FireBall()
{
    NodeName = TEXT("Fire Ball Attack");
}

EBTNodeResult::Type UBTTask_FireBall::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
    BossMonster->SpawnFireBall(TargetLocation);

    return EBTNodeResult::Succeeded;
}
