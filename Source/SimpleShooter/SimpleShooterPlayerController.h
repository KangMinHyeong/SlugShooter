// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASimpleShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	virtual void SetupInputComponent() override;

	void ChangeInputMode(bool bGameMode = true);

	void RetryLevel();

	void CannotSave();

	void RemoverWidget();

	void SpawnBossHPUI();

	void RemoveBossHPUI();

	void SpawnGhostModeUI();

	void RemoveGhostModeUI();

	void ClearGameEnd();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGameplayWidget> MenuWidgetClass;

private:
	FTimerHandle TimerHandle;
	FTimerHandle SaveTimerHandle;

	UPROPERTY(EditAnywhere)
	float RespawnTime = 0.5f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CantSave;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> Reload;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> BossHP;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GhostMode;

	UPROPERTY(EditAnywhere)
	UUserWidget* HUDWiget;

	UPROPERTY(EditAnywhere)
	UUserWidget* CannotSaveWiget;

	UPROPERTY(EditAnywhere)
	UUserWidget* ReloadWiget;

	UPROPERTY(EditAnywhere)
	UUserWidget* BossHPWiget;

	UPROPERTY(EditAnywhere)
	UUserWidget* GhostModeWidget;
	
	FInputModeGameOnly GameInputMode;

	UPROPERTY()
	UGameplayWidget* MenuWidget;

	FInputModeUIOnly UIInputMode;

	UPROPERTY()
	class AShooterPlayerState* MyPlayerState;

private:
	void OnGamePause();
};
