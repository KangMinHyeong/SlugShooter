// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_OilTrap.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_OilTrap : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_OilTrap();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);		
};
