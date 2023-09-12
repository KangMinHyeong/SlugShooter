// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ClearGodMode.h"

UBTTaskNode_ClearGodMode::UBTTaskNode_ClearGodMode()
{
    NodeName = TEXT("Clear GodMode");
}

EBTNodeResult::Type UBTTaskNode_ClearGodMode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->bGodMode = false;

    return EBTNodeResult::Succeeded;
}
