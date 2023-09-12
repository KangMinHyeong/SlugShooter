// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationUpdate.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "ShooterCharacter.h"

UBTService_PlayerLocationUpdate::UBTService_PlayerLocationUpdate()
{
    NodeName = TEXT("Update PlayerLocation");
}

void UBTService_PlayerLocationUpdate::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(PlayerPawn == nullptr) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}
