// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OilTrap.generated.h"

UCLASS()
class SIMPLESHOOTER_API AOilTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOilTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void DestroyTrap();
	void HitAction();
	AActor* GetAcceptableActor() const;
	void OnCharacterOverlap(AActor* OtherActor);
	void FireStart();
	void Explosion();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	USceneComponent* TrapRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class USphereComponent* TrapComponent;

	UPROPERTY(EditAnywhere)
	float Damage = 50;

	FTimerHandle DestroyTimerHandle;
	FTimerHandle TimerHandle;
	FTimerHandle FireTimerHandle;
	FTimerHandle ExplosionTimerHandle;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 6.0f;

	UPROPERTY(EditAnywhere)
	float DestroyTime_Min = 6.0f;

	UPROPERTY(EditAnywhere)
	float DestroyTime_Max = 25.0f;

	UPROPERTY(EditAnywhere)
	float HitableTime = 0.1f;

	UPROPERTY(EditAnywhere)
	float FireTime = 0.2f;

	UPROPERTY(EditAnywhere)
	float ExplosionTime_Min = 5.0f;

	UPROPERTY(EditAnywhere)
	float ExplosionTime_Max = 15.0f;

	UPROPERTY(EditAnywhere)
	float HomingSpeed = 2500.0f;

	UPROPERTY(EditAnywhere)
	float StunTime = 10.0f;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* SmokeParticle;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* HitParticle;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere)
	FVector FireScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UPROPERTY(EditAnywhere)
	bool bExplosionTrap = false;

	UPROPERTY(EditAnywhere)
	bool bOilTrap = false;

	UPROPERTY(EditAnywhere)
	bool bHomingGrenade = true;

	UPROPERTY(EditAnywhere)
	bool bEMP = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> BombCameraShakeClass;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	USoundBase* OilSound;

	UPROPERTY(EditAnywhere)
	USoundBase* OilFireSound;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;
};
