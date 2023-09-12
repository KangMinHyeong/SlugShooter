// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossHPisQuarter.h"

UBTDecorator_BossHPisQuarter::UBTDecorator_BossHPisQuarter()
{
    NodeName = TEXT("BossHPisQuarter");
}

bool UBTDecorator_BossHPisQuarter::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    float HPPercent = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if(HPPercent <= 0.25)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
	return bResult;
}
