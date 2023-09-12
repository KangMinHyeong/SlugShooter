// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo.h"
#include "MonsterSkill.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AMonsterSkill : public AAmmo
{
	GENERATED_BODY()
public:
	AMonsterSkill();

protected:
	virtual void BeginPlay() override;

public:
	void OnCharacterOverlap(AActor* OtherActor);

	virtual void Tick(float DeltaTime) override;

	void MonsterSkillOnHit(AActor *OtherActor);

	void DestroyReady();

private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	AActor* GetAcceptableActor() const;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* SkillTrigger;

	FTimerHandle SkillTimerHandle;

	UPROPERTY(EditAnywhere)
	float SkillDuration = 3.5f;
	
	UPROPERTY(EditAnywhere)
	float Speed = 200.0f;
	
};
