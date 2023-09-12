// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RushAttack.h"

UBTTaskNode_RushAttack::UBTTaskNode_RushAttack()
{
    NodeName = TEXT("RushAttack");
}

EBTNodeResult::Type UBTTaskNode_RushAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->RushAttack();

    return EBTNodeResult::Succeeded;
}
