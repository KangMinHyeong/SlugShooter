// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetGodMode.h"

UBTTaskNode_SetGodMode::UBTTaskNode_SetGodMode()
{
    NodeName = TEXT("SetGodMode");
}

EBTNodeResult::Type UBTTaskNode_SetGodMode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->bGodMode = true;

    return EBTNodeResult::Succeeded;
}
