// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_EnhanceRush.h"

UBTTaskNode_EnhanceRush::UBTTaskNode_EnhanceRush()
{
    NodeName = TEXT("Enhance Rush");
}

EBTNodeResult::Type UBTTaskNode_EnhanceRush::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->EnHanceRush();

    return EBTNodeResult::Succeeded;
}
