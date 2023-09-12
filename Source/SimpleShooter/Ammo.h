// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class SIMPLESHOOTER_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HitAction();

	void SetTargeting(FVector TargetLocation);

	void DestroyAmmo();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 50;

	UPROPERTY(EditAnywhere)
	float HomingSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* SmokeParticle;

	FTimerHandle TimerHandle;
	FTimerHandle AmmoTimerHandle;

	UPROPERTY(EditAnywhere)
	float HitableTime = 1;

	UPROPERTY(EditAnywhere)
	float AmmoDuration = 4.5f;

	UPROPERTY(EditAnywhere)
	float TraceSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool TraceOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bFireBall = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bHomingMissile = false;

	UPROPERTY(EditAnywhere)
	float MaxHomingDistance = 5000.0f;

	FVector TargetLoc;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;

};
