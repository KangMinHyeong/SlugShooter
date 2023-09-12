// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ThrowExplosionTrap.h"

UBTTaskNode_ThrowExplosionTrap::UBTTaskNode_ThrowExplosionTrap()
{
    NodeName = TEXT("ThrowExplosionTrap");
}

EBTNodeResult::Type UBTTaskNode_ThrowExplosionTrap::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->ThrowExplosionTrap();

    return EBTNodeResult::Succeeded;
}
