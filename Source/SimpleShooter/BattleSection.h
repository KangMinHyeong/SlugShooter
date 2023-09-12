// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleSection.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABattleSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleSection();

private:
	enum class EBattleState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnCharacterOverlap(AActor* OtherActor);

	void SetBattle(EBattleState NewState);

	void SetCloseDoor();

	void SpawnMonster();

	void SpawnMonsterbyNumbers(int32 Numbers, USceneComponent* SpawnPoint, bool BigMonster);

	void SpawnRangeMonsterbyNumbers(int32 Numbers, USceneComponent* SpawnPoint);

	void SpawnEpicRangeMonsterbyNumbers(int32 Numbers);

	void SpawnMiddleBossbyNumbers(int32 Numbers);

	UFUNCTION()
	void OnMonsterDestroy(AActor* Monster);

	UFUNCTION()
	void OnRangeMonsterDestroy(AActor* Monster);

	UFUNCTION()
	void OnMiddleBossDestroy(AActor* Monster);

	void ClearStage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UBoxComponent* StartCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UBoxComponent* DestinationCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* StartDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* DestinationDoor;

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	AActor* GetAcceptableActor() const;

	FTimerHandle DoorTimerHandle;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle ClearTimerHandle;

	UPROPERTY(EditAnywhere)
	int32 MonsterNumbers_1 = 1;

	UPROPERTY(EditAnywhere)
	int32 MonsterNumbers_2 = 1;

	UPROPERTY(EditAnywhere)
	int32 MonsterNumbers_3 = 1;

	UPROPERTY(EditAnywhere)
	int32 MonsterNumbers_4 = 1;

	UPROPERTY(EditAnywhere)
	int32 MonsterNumbers_5 = 1;

	UPROPERTY(EditAnywhere)
	int32 RangeMonsterNumbers = 2;

	UPROPERTY(EditAnywhere)
	int32 RangeMonsterNumbers_2 = 2;

	UPROPERTY(EditAnywhere)
	int32 EpicRangeMonsterNumbers = 1;

	UPROPERTY(EditAnywhere)
	int32 MiddleBossNumbers = 1;

	UPROPERTY(EditAnywhere)
	float MonsterSpawnDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	float ClearTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AGameMonster> MonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AGameMonster> MonsterClass_2;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AMiddleBoss> MiddleBossClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AMiddleBoss> MiddleBossClass_2;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ARangeMonsterCharacter> RangeMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ARangeMonsterCharacter> EpicRangeMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AShooterCharacter> CharacterClass;

	UPROPERTY(EditAnywhere)
	int32 StageTag = -1;

	int32 SaveStageTag = 0;

	UPROPERTY(VisibleAnywhere)
	bool bClearStage = false;

	UPROPERTY(EditAnywhere)
	bool bGrux = true;

	bool bBattle = false;

	UPROPERTY(EditAnywhere)
	USceneComponent* MonsterSpawnPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* MonsterSpawnPoint_2;

	UPROPERTY(EditAnywhere)
	USceneComponent* MonsterSpawnPoint_3;

	UPROPERTY(EditAnywhere)
	USceneComponent* MonsterSpawnPoint_4;

	UPROPERTY(EditAnywhere)
	USceneComponent* MonsterSpawnPoint_5;

	UPROPERTY(EditAnywhere)
	USceneComponent* MiddleBossSpawnPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* RangeMonsterSpawnPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* RangeMonsterSpawnPoint_2;

	UPROPERTY(EditAnywhere)
	USceneComponent* EpicRangeMonsterSpawnPoint;

};
