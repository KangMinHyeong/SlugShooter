// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void NewGame();

	UFUNCTION()
	void Continue();

public:
	UPROPERTY()
	class UButton* NewGameBtn;

	UPROPERTY()
	class UButton* ContinueBtn;

};
