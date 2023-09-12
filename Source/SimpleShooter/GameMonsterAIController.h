// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AGameMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	bool MonsterIsDead() const;
	bool RangeMonsterIsDead() const;
	bool MiddleBossIsDead() const;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

};
