// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ClearBreathReady.h"

UBTTaskNode_ClearBreathReady::UBTTaskNode_ClearBreathReady()
{
    NodeName = TEXT("Clear Breath Ready");
}

EBTNodeResult::Type UBTTaskNode_ClearBreathReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->bBreathReady = false;

    return EBTNodeResult::Succeeded;
}
