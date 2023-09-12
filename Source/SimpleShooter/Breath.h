// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breath.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABreath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnCharacterOverlap(AActor* OtherActor);
	AActor* GetAcceptableActor() const;
	void BreathPlay();
	void BreathOnHit(AActor *OtherActor);
	void DestroyBreath();
	void FireSoundLoop();

private:
	UPROPERTY(EditAnywhere)
	class USceneComponent* BreathRoot;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BreathTrigger;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* BreathEffect;

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	FTimerHandle DestroyTimerHandle;
	FTimerHandle SoundTimerHandle;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float InitDelay = 2.5f;
	
	UPROPERTY(EditAnywhere)
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere)
	float SoundTime = 0.5f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;
	
};
