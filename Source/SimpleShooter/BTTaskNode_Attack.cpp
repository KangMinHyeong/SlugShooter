// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AGameMonster* Monster = Cast<AGameMonster>(OwnerComp.GetAIOwner()->GetPawn());
    if(Monster == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    Monster->PlayAttack();

    return EBTNodeResult::Succeeded;
}
