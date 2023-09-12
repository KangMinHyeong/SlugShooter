// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShotGun : public AGun
{
	GENERATED_BODY()
	
public:
	AShotGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OperatingSkiils() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SkillSpawningPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ASkillAmmo> SkillClass;

};
