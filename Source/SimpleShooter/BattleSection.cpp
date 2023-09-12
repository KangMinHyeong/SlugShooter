// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSection.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "GameMonster.h"
#include "EngineUtils.h"
#include "GameMonsterAIController.h"
#include "ShooterAIController.h"
#include "ShooterGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "RangeMonsterCharacter.h"
#include "GameMonster.h"
#include "SimpleShooterPlayerController.h"
#include "MiddleBoss.h"


// Sets default values
ABattleSection::ABattleSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	StartCap = CreateDefaultSubobject<UBoxComponent>(TEXT("Start"));
	DestinationCap = CreateDefaultSubobject<UBoxComponent>(TEXT("Destination"));
	StartDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Start Door"));
	DestinationDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination Door"));
	MonsterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MonsterSpawnPoint"));
	MonsterSpawnPoint_2 = CreateDefaultSubobject<USceneComponent>(TEXT("MonsterSpawnPoint2"));
	MonsterSpawnPoint_3 = CreateDefaultSubobject<USceneComponent>(TEXT("MonsterSpawnPoint_3"));
	MonsterSpawnPoint_4 = CreateDefaultSubobject<USceneComponent>(TEXT("MonsterSpawnPoint_4"));
	MonsterSpawnPoint_5 = CreateDefaultSubobject<USceneComponent>(TEXT("MonsterSpawnPoint_5"));
	MiddleBossSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MiddleBossSpawnPoint"));
	RangeMonsterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RangeMonsterSpawnPoint"));
	RangeMonsterSpawnPoint_2 = CreateDefaultSubobject<USceneComponent>(TEXT("RangeMonsterSpawnPoint_2"));
	EpicRangeMonsterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EpicRangeMonsterSpawnPoint"));

	RootComponent = RootScene;
	StartCap->SetupAttachment(RootScene);
	StartDoor->SetupAttachment(RootScene);
	DestinationCap->SetupAttachment(RootScene);
	DestinationDoor->SetupAttachment(RootScene);
	MonsterSpawnPoint->SetupAttachment(RootScene);
	RangeMonsterSpawnPoint->SetupAttachment(RootScene);
	RangeMonsterSpawnPoint_2->SetupAttachment(RootScene);
	EpicRangeMonsterSpawnPoint->SetupAttachment(RootScene);
	MonsterSpawnPoint_2->SetupAttachment(RootScene);
	MonsterSpawnPoint_3->SetupAttachment(RootScene);
	MonsterSpawnPoint_4->SetupAttachment(RootScene);
	MonsterSpawnPoint_5->SetupAttachment(RootScene);
	MiddleBossSpawnPoint->SetupAttachment(RootScene);
}

// Called when the game starts or when spawned
void ABattleSection::BeginPlay()
{
	Super::BeginPlay();


	
	AShooterGameStateBase* GameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
	if(GameState != nullptr)
	{
		if(GameState->ClearStageTags.IsEmpty())
		{
			UE_LOG(LogTemp, Display, TEXT("Your empty"));
		}
		else
		{
			TArray<int32> SaveTags = GameState->ClearStageTags;
			for(int32 SaveGameData : SaveTags)
			{
				if(SaveGameData == StageTag)
				{
					UE_LOG(LogTemp, Display, TEXT("Your dest"));
					Destroy();
				}
			}
		}
	}
	if(GameState == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Your GameState nullptr"));
	}

	StartDoor->SetHiddenInGame(true);
	StartDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestinationDoor->SetHiddenInGame(true);
	DestinationDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called every frame
void ABattleSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {   
        AActor::SetActorTickEnabled(false);
        OnCharacterOverlap(Actor);
    }
    else
    {}
}


void ABattleSection::SetBattle(EBattleState NewState)
{
	switch (NewState)
	{
	case EBattleState::READY:
	{
		GetWorld()->GetTimerManager().SetTimer(DoorTimerHandle, this, &ABattleSection::SetCloseDoor, 0.1f);
		bBattle = true;
		AShooterGameStateBase* GameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
		if(GameState == nullptr)
		{
			break;
		}
		GameState->IsBattleMode(bBattle);
		break;
	}
	case EBattleState::BATTLE:
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABattleSection::SpawnMonster, MonsterSpawnDelay);
		break;
	}
	case EBattleState::COMPLETE:
	{
		GetWorld()->GetTimerManager().SetTimer(ClearTimerHandle, this, &ABattleSection::ClearStage, ClearTime);
		bBattle = false;
		AShooterGameStateBase* GameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(this));
		if(GameState == nullptr)
		{
			break;
		}
		GameState->SaveClearStageWithTag(StageTag);
		GameState->IsBattleMode(bBattle);
		break;
	}
	}

}

void ABattleSection::SetCloseDoor()
{
	StartDoor->SetHiddenInGame(false);
	StartDoor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestinationDoor->SetHiddenInGame(false);
	DestinationDoor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetBattle(EBattleState::BATTLE);
	GetWorld()->GetTimerManager().ClearTimer(DoorTimerHandle);   
}

void ABattleSection::SpawnMonster()
{
	for(int32 i = 0; i < MonsterNumbers_1; i++)
	{
		if(i%2 == 0)
		{
			SpawnMonsterbyNumbers(-i/2, MonsterSpawnPoint, false);
		}
		else
		{
			SpawnMonsterbyNumbers(i/2 + 1, MonsterSpawnPoint, false);
		}
	}

	for(int32 q = 0; q < MonsterNumbers_2; q++)
	{
		if(q%2 == 0)
		{
			SpawnMonsterbyNumbers(-q/2, MonsterSpawnPoint_2, false);
		}
		else
		{
			SpawnMonsterbyNumbers(q/2 + 1, MonsterSpawnPoint_2, false);
		}
	}

	for(int32 x = 0; x < MonsterNumbers_3; x++)
	{
		if(x%2 == 0)
		{
			SpawnMonsterbyNumbers(-x/2, MonsterSpawnPoint_3, false);
		}
		else
		{
			SpawnMonsterbyNumbers(x/2 + 1, MonsterSpawnPoint_3, false);
		}
	}

	for(int32 y = 0; y < MonsterNumbers_4; y++)
	{
		if(y%2 == 0)
		{
			SpawnMonsterbyNumbers(-y/2, MonsterSpawnPoint_4, true);
		}
		else
		{
			SpawnMonsterbyNumbers(y/2 + 1, MonsterSpawnPoint_4, true);
		}
	}

	for(int32 z = 0; z < MonsterNumbers_5; z++)
	{
		if(z%2 == 0)
		{
			SpawnMonsterbyNumbers(-z/2, MonsterSpawnPoint_5, true);
		}
		else
		{
			SpawnMonsterbyNumbers(z/2 + 1, MonsterSpawnPoint_5, true);
		}
	}

	for(int32 t = 0; t < RangeMonsterNumbers; t++)
	{
		if(t%2 == 0)
		{
			SpawnRangeMonsterbyNumbers(-t/2, RangeMonsterSpawnPoint);
		}
		else
		{
			SpawnRangeMonsterbyNumbers(t/2+1, RangeMonsterSpawnPoint);
		}
		
	}

	for(int32 s = 0; s < RangeMonsterNumbers_2; s++)
	{
		if(s%2 == 0)
		{
			SpawnRangeMonsterbyNumbers(-s/2, RangeMonsterSpawnPoint_2);
		}
		else
		{
			SpawnRangeMonsterbyNumbers(s/2+1, RangeMonsterSpawnPoint_2);
		}
		
	}

	for(int32 r = 0; r < EpicRangeMonsterNumbers; r++)
	{
		if(r%2 == 0)
		{
			SpawnEpicRangeMonsterbyNumbers(-r/2);
		}
		else
		{
			SpawnEpicRangeMonsterbyNumbers(r/2+1);
		}
	}

	for(int32 p = 0; p < MiddleBossNumbers; p++)
	{
		if(p%2 == 0)
		{
			SpawnMiddleBossbyNumbers(-p/2);
		}
		else
		{
			SpawnMiddleBossbyNumbers(p/2+1);
		}
	}
}

void ABattleSection::SpawnMonsterbyNumbers(int32 Numbers, USceneComponent* SpawnPoint, bool BigMonster)
{
	float Rand = FMath::RandRange(-100.0f , 100.0f);
	if(!BigMonster)
	{
		AGameMonster* Monsters = GetWorld()->SpawnActor<AGameMonster>(MonsterClass, SpawnPoint->GetComponentLocation() + FVector::RightVector*150.0f * Numbers + FVector::ForwardVector * Rand, FRotator::ZeroRotator);
		if(Monsters != nullptr)
		{
			Monsters->OnDestroyed.AddDynamic(this, &ABattleSection::OnMonsterDestroy);
		}
	}
	else
	{
		AGameMonster* Monsters_Big = GetWorld()->SpawnActor<AGameMonster>(MonsterClass_2, SpawnPoint->GetComponentLocation() + FVector::RightVector*150.0f * Numbers + FVector::ForwardVector * Rand, FRotator::ZeroRotator);
		if(Monsters_Big != nullptr)
		{
			Monsters_Big->OnDestroyed.AddDynamic(this, &ABattleSection::OnMonsterDestroy);
		}
	}

}

void ABattleSection::SpawnRangeMonsterbyNumbers(int32 Numbers, USceneComponent* SpawnPoint)
{
	float Rand = FMath::RandRange(-100.0f , 100.0f);
	ARangeMonsterCharacter* Monsters = GetWorld()->SpawnActor<ARangeMonsterCharacter>(RangeMonsterClass, SpawnPoint->GetComponentLocation() + FVector::RightVector*185.0f * RangeMonsterNumbers + FVector::ForwardVector * Rand, FRotator::ZeroRotator);
	if(Monsters != nullptr)
	{
		Monsters->OnDestroyed.AddDynamic(this, &ABattleSection::OnRangeMonsterDestroy);
	}
}

void ABattleSection::SpawnEpicRangeMonsterbyNumbers(int32 Numbers)
{
	float Rand = FMath::RandRange(-100.0f , 100.0f);
	ARangeMonsterCharacter* Monsters = GetWorld()->SpawnActor<ARangeMonsterCharacter>(EpicRangeMonsterClass, EpicRangeMonsterSpawnPoint->GetComponentLocation() + FVector::RightVector*185.0f * EpicRangeMonsterNumbers + FVector::ForwardVector * Rand, FRotator::ZeroRotator);
	if(Monsters != nullptr)
	{
		Monsters->OnDestroyed.AddDynamic(this, &ABattleSection::OnRangeMonsterDestroy);
	}
}

void ABattleSection::SpawnMiddleBossbyNumbers(int32 Numbers)
{
	if(bGrux)
	{
		AMiddleBoss* Monsters = GetWorld()->SpawnActor<AMiddleBoss>(MiddleBossClass, MiddleBossSpawnPoint->GetComponentLocation() + FVector::RightVector*185.0f * MiddleBossNumbers, FRotator::ZeroRotator);
		if(Monsters != nullptr)
		{
			Monsters->OnDestroyed.AddDynamic(this, &ABattleSection::OnMiddleBossDestroy);
		}
	}
	else
	{
		AMiddleBoss* Monsters_2 = GetWorld()->SpawnActor<AMiddleBoss>(MiddleBossClass_2, MiddleBossSpawnPoint->GetComponentLocation() + FVector::RightVector*185.0f * MiddleBossNumbers, FRotator::ZeroRotator);
		if(Monsters_2 != nullptr)
		{
			Monsters_2->OnDestroyed.AddDynamic(this, &ABattleSection::OnMiddleBossDestroy);
		}		
	}

}

void ABattleSection::OnMonsterDestroy(AActor *Monster)
{
	AGameMonster* DestroyedMonsyer = Cast<AGameMonster>(Monster);
	if(DestroyedMonsyer == nullptr) return;

	ASimpleShooterPlayerController* ShooterController = Cast<ASimpleShooterPlayerController>(DestroyedMonsyer->LastHitBy);
	if(ShooterController == nullptr) return;

	for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!AIController->IsDead()) return;
    }

	for(AGameMonsterAIController* MonsterAIController : TActorRange<AGameMonsterAIController>(GetWorld()))
    {
        if(!MonsterAIController->MonsterIsDead()) return;
		if(!MonsterAIController->RangeMonsterIsDead()) return;
    }
	
	SetBattle(EBattleState::COMPLETE);
}

void ABattleSection::OnRangeMonsterDestroy(AActor *Monster)
{
	ARangeMonsterCharacter* DestroyedRangeMonsyer = Cast<ARangeMonsterCharacter>(Monster);
	if(DestroyedRangeMonsyer == nullptr) return;

	ASimpleShooterPlayerController* ShooterController = Cast<ASimpleShooterPlayerController>(DestroyedRangeMonsyer->LastHitBy);
	if(ShooterController == nullptr) return;

	for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!AIController->IsDead()) return;
    }

	for(AGameMonsterAIController* MonsterAIController : TActorRange<AGameMonsterAIController>(GetWorld()))
    {
        if(!MonsterAIController->MonsterIsDead()) return;
		if(!MonsterAIController->RangeMonsterIsDead()) return;
    }

	SetBattle(EBattleState::COMPLETE);
}

void ABattleSection::OnMiddleBossDestroy(AActor *Monster)
{
	AMiddleBoss* DestroyedMiddleBoss = Cast<AMiddleBoss>(Monster);
	if(DestroyedMiddleBoss == nullptr) return;

	ASimpleShooterPlayerController* ShooterController = Cast<ASimpleShooterPlayerController>(DestroyedMiddleBoss->LastHitBy);
	if(ShooterController == nullptr) return;

	for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!AIController->IsDead()) return;
    }

	for(AGameMonsterAIController* MonsterAIController : TActorRange<AGameMonsterAIController>(GetWorld()))
    {
        if(!MonsterAIController->MonsterIsDead()) return;
		if(!MonsterAIController->RangeMonsterIsDead()) return;
		if(!MonsterAIController->MiddleBossIsDead()) return;
    }

	SetBattle(EBattleState::COMPLETE);
}

void ABattleSection::ClearStage()
{
	StartDoor->SetHiddenInGame(true);
	StartDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestinationDoor->SetHiddenInGame(true);
	DestinationDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().ClearTimer(ClearTimerHandle);
}

AActor *ABattleSection::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(AcceptableActorTag))
        {   
            return Actor;
        }
    }    

    return nullptr;
}

void ABattleSection::OnCharacterOverlap(AActor *OtherActor)
{
	SetBattle(EBattleState::READY);
}