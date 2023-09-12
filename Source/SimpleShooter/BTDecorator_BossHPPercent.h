// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_BossHPPercent.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTDecorator_BossHPPercent : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_BossHPPercent();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
