// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MiddleBoss_StunAttack.h"
#include "MiddleBoss.h"

UBTTaskNode_MiddleBoss_StunAttack::UBTTaskNode_MiddleBoss_StunAttack()
{
    NodeName = TEXT("Stun Attack");
}

EBTNodeResult::Type UBTTaskNode_MiddleBoss_StunAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    MiddleBoss->StunAttack();

    return EBTNodeResult::Succeeded;
}