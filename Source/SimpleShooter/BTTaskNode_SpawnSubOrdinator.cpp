// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SpawnSubOrdinator.h"

UBTTaskNode_SpawnSubOrdinator::UBTTaskNode_SpawnSubOrdinator()
{
    NodeName = TEXT("Spawn SubOrdinator");
}

EBTNodeResult::Type UBTTaskNode_SpawnSubOrdinator::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->SpawnSubReady();

    return EBTNodeResult::Succeeded;
}
