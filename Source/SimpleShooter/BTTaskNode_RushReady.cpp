// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RushReady.h"

UBTTaskNode_RushReady::UBTTaskNode_RushReady()
{
    NodeName = TEXT("RushReady");
}

EBTNodeResult::Type UBTTaskNode_RushReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->RushReady();

    return EBTNodeResult::Succeeded;
}
