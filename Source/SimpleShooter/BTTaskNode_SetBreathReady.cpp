// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetBreathReady.h"

UBTTaskNode_SetBreathReady::UBTTaskNode_SetBreathReady()
{
    NodeName = TEXT("Set Breath Ready");
}

EBTNodeResult::Type UBTTaskNode_SetBreathReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->PlayBreath();

    return EBTNodeResult::Succeeded;
}
