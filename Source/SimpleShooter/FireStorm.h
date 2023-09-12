// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireStorm.generated.h"

UCLASS()
class SIMPLESHOOTER_API AFireStorm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireStorm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void FireStormReady();
	void FireStormPlay();
	void FireReset();
	void OnCharacterOverlap(AActor* OtherActor);
	AActor* GetAcceptableActor() const;
	void FireOnHit(AActor *OtherActor);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* FireTrigger;

	UPROPERTY(EditAnywhere)
	UBoxComponent* FireTrigger_None;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* FireEffect;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* FireEffect_Ready;

	FTimerHandle FireReadyTimerHandle;
	FTimerHandle FirePlayTimerHandle;
	FTimerHandle FirePlayResetTimerHandle;

	UPROPERTY(EditAnywhere)
	float FireReadyTime = 25.0f;

	UPROPERTY(EditAnywhere)
	float FirePlayTime = 2.8f;

	UPROPERTY(EditAnywhere)
	float FireResetTime = 6.0f;

	UPROPERTY(EditAnywhere)
	float Damage = 15.0f;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;

};
