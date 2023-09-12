// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MiddleBoss_ShieldMode.h"

UBTTaskNode_MiddleBoss_ShieldMode::UBTTaskNode_MiddleBoss_ShieldMode()
{
    NodeName = TEXT("Shield Mode");
}

EBTNodeResult::Type UBTTaskNode_MiddleBoss_ShieldMode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    MiddleBoss->ShieldMode();

    return EBTNodeResult::Succeeded;
}
