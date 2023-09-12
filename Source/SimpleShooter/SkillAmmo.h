// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo.h"
#include "SkillAmmo.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASkillAmmo : public AAmmo
{
	GENERATED_BODY()
public:
	ASkillAmmo();

protected:
	virtual void BeginPlay() override;

public:
	void OnCharacterOverlap(AActor* OtherActor);

	virtual void Tick(float DeltaTime) override;

	void ReturnHitMode();

	void SkillOnHit(AActor *OtherActor);

	void DestroySkillAmmo();

	void DestroyReady();


private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	AActor* GetAcceptableActor() const;

	UPROPERTY(EditAnywhere, Category = Effect)
	UParticleSystem* DestroyEffect;

	FTimerHandle SkillTimerHandle;

	UPROPERTY(EditAnywhere)
	float SkillDuration = 3.5f;

	UPROPERTY(EditAnywhere)
	float DestroyDuration = 0.2f;


};
