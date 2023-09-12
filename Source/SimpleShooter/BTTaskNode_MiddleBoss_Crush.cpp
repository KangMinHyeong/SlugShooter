// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MiddleBoss_Crush.h"

UBTTaskNode_MiddleBoss_Crush::UBTTaskNode_MiddleBoss_Crush()
{
    NodeName = TEXT("Crush");
}

EBTNodeResult::Type UBTTaskNode_MiddleBoss_Crush::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AMiddleBoss* MiddleBoss = Cast<AMiddleBoss>(OwnerComp.GetAIOwner()->GetPawn());
    if(MiddleBoss == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    MiddleBoss->CrushAttack();

    return EBTNodeResult::Succeeded;
}