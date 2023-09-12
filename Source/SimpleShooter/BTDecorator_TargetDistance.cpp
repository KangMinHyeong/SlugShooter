// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_TargetDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_TargetDistance::UBTDecorator_TargetDistance()
{
    NodeName = TEXT("TargetDistance");
}

bool UBTDecorator_TargetDistance::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    float TargetDist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if(TargetDist >= BossMonster->TargetRangeDist)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
	return bResult;
}
