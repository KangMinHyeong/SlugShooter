// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_NormalAttack.h"

UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack()
{
    NodeName = TEXT("Normal Attack");
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->NormalAttack();

    return EBTNodeResult::Succeeded;
}
