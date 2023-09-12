// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MiddleBoss_VelocityUp.h"

UBTTaskNode_MiddleBoss_VelocityUp::UBTTaskNode_MiddleBoss_VelocityUp()
{
    NodeName = TEXT("Velocity Up");
}

EBTNodeResult::Type UBTTaskNode_MiddleBoss_VelocityUp::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    MiddleBoss->VelocityUp();

    return EBTNodeResult::Succeeded;
}
