// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"

UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* Shooter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(Shooter == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    Shooter->Shoot();

    return EBTNodeResult::Succeeded;
}
