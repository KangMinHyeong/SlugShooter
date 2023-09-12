// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	AKillEmAllGameMode();

	virtual void PawnKilled(APawn* PawnKilled) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;	

	virtual void StartPlay() override;

	UFUNCTION(BlueprintPure)
	AActor* ReturnBossAIController(); 
private:
	void EndGame(bool bIsPlayerPawn);
};
