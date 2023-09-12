// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void Resume();

	UFUNCTION()
	void ReturnToTitle();

	UFUNCTION()
	void Retry();

	UFUNCTION()
	void Save();

public:
	UPROPERTY()
	class UButton* ResumeBtn;

	UPROPERTY()
	UButton* ReturnToTitleBtn;
	
	UPROPERTY()
	UButton* RetryBtn;

	UPROPERTY()
	UButton* SaveBtn;
};
