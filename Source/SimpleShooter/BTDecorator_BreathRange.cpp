// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BreathRange.h"

UBTDecorator_BreathRange::UBTDecorator_BreathRange()
{
    NodeName = TEXT("Breath Range");
}

bool UBTDecorator_BreathRange::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    float TargetDist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if(TargetDist >= BossMonster->BreathRangeDist)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
	return bResult;
}
