// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal.h"
#include "LevelPortal.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ALevelPortal : public APortal
{
	GENERATED_BODY()
	
public:
	virtual void TelePort() override;

private:
	UPROPERTY(EditAnywhere)
	FName NextLevelName;
};
