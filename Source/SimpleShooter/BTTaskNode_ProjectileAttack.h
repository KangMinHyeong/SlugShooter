// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskNode_ProjectileAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ProjectileAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
