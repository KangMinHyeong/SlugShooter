// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Breath.h"

UBTTaskNode_Breath::UBTTaskNode_Breath()
{
    NodeName = TEXT("Breath");
}

EBTNodeResult::Type UBTTaskNode_Breath::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->Breathing();

    return EBTNodeResult::Succeeded;
}
