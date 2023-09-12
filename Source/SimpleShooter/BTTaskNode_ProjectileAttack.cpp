// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ProjectileAttack.h"
#include "RangeMonsterCharacter.h"

UBTTaskNode_ProjectileAttack::UBTTaskNode_ProjectileAttack()
{
    NodeName = TEXT("Projectile_Attack");
}

EBTNodeResult::Type UBTTaskNode_ProjectileAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ARangeMonsterCharacter* Monster = Cast<ARangeMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(Monster == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    Monster->PlayProjectileAttack();

    return EBTNodeResult::Succeeded;
}
