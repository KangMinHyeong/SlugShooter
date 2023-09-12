// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossHPPercent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_BossHPPercent::UBTDecorator_BossHPPercent()
{    
    NodeName = TEXT("BossHPPercent");
}

bool UBTDecorator_BossHPPercent::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
	if (BossMonster == nullptr) return false;

    float  HPPercent = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if(HPPercent <= 0.80)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
	return bResult;
}
