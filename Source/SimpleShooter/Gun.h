// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	virtual void PullTrigger();

	virtual void OperatingSkiils();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	bool GunTrace(FHitResult& HitResult, FVector& ParticleRotate);

	AController* OwnerController();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	float AttackSpeedTime = 0.8f;

	UPROPERTY(EditAnywhere)
	int32 RemainBullet = 40;

	UPROPERTY(EditAnywhere)
	int32 MaximumBullet = 40;

	UPROPERTY(EditAnywhere)
	int32 OriginBullet = 40;

	UPROPERTY(EditAnywhere)
	int32 GunNumber = 1;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ShootParticle;

	UPROPERTY(EditAnywhere)
	class USoundBase* ShootSound;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 1500;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere)
	float AmmoScale = 5.0f;

	UPROPERTY(EditAnywhere)
	float AmmoSpeed = 1800.0f;

	UPROPERTY(EditAnywhere)
	float ShootRecoil_Max = 0.5f;

	UPROPERTY(EditAnywhere)
	float ShootRecoil_Max_Yaw = 0.5f;

	UPROPERTY(EditAnywhere)
	bool ExplosiveAttack = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ShootCameraShakeClass;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BulletSpawningPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AAmmo> AmmoClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AOilTrap> ExplosionAmmoClass;

};
