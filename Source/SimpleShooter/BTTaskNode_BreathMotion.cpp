// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_BreathMotion.h"

UBTTaskNode_BreathMotion::UBTTaskNode_BreathMotion()
{
    NodeName = TEXT("Breath Motion");
}

EBTNodeResult::Type UBTTaskNode_BreathMotion::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->SpawnBreath();

    return EBTNodeResult::Succeeded;
}

