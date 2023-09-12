// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPPortion.generated.h"

UCLASS()
class SIMPLESHOOTER_API AHPPortion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPPortion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnCharacterOverlap(AActor* OtherActor);

	void HPRecovery();

private:

	UPROPERTY(EditAnywhere)
	class  UCapsuleComponent* PorationCapsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PorationMesh;

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	AActor* GetAcceptableActor() const;

	FTimerHandle PortTimerHandle;

	UPROPERTY(EditAnywhere)
	float RecoveryHP = 50;

	class AShooterCharacter* Character;
};
