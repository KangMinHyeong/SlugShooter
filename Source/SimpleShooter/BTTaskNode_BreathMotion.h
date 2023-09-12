// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_BreathMotion.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskNode_BreathMotion : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_BreathMotion();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);		
};
