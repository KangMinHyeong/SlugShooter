// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BreathCondition.h"

UBTDecorator_BreathCondition::UBTDecorator_BreathCondition()
{
    NodeName = TEXT("Breath Condition");
}

bool UBTDecorator_BreathCondition::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    bool Condition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());

	if(Condition)
    {
        bResult = true;
    }
    else
    {
        bResult = false;
    }
	return bResult;
}
