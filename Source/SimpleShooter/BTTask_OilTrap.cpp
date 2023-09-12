// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_OilTrap.h"

UBTTask_OilTrap::UBTTask_OilTrap()
{
    NodeName = TEXT("OilTrap");
}

EBTNodeResult::Type UBTTask_OilTrap::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
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

    BossMonster->ReadyOilTrap();

    return EBTNodeResult::Succeeded;
}
