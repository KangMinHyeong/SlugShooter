// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotatingBoss.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_RotatingBoss : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RotatingBoss();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	

};
