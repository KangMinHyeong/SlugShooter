// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ExplosionTrapReady.h"

UBTTaskNode_ExplosionTrapReady::UBTTaskNode_ExplosionTrapReady()
{
    NodeName = TEXT("ExplosionTrapReady");
}

EBTNodeResult::Type UBTTaskNode_ExplosionTrapReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->ExplosionTrapReady();

    return EBTNodeResult::Succeeded;
}