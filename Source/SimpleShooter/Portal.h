// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class SIMPLESHOOTER_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	class  UCapsuleComponent* PortalCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* DestinationCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DestinationMesh;

public:
	void OnCharacterOverlap(AActor* OtherActor);

	virtual void TelePort();
	
	void TickEnable();

private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	FVector ScaleVec;

	AActor* GetAcceptableActor() const;

	FTimerHandle TimerHandle;

	

public:
	FTimerHandle PortTimerHandle;
	
	class AShooterCharacter* Character;
};
