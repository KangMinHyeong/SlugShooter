// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossHPisHalf.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_BossHPisHalf::UBTDecorator_BossHPisHalf()
{
    NodeName = TEXT("BossHPisHalf");
}

bool UBTDecorator_BossHPisHalf::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    float HPPercent = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if(HPPercent <= 0.5)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
	return bResult;
}