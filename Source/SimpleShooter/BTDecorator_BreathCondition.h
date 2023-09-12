// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_BreathCondition.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTDecorator_BreathCondition : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_BreathCondition();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
};
