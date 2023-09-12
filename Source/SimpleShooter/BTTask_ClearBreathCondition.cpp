// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBreathCondition.h"

UBTTask_ClearBreathCondition::UBTTask_ClearBreathCondition()
{
    NodeName = TEXT("Clear BreathCondition");
}

EBTNodeResult::Type UBTTask_ClearBreathCondition::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
    

    return EBTNodeResult::Succeeded;
}
