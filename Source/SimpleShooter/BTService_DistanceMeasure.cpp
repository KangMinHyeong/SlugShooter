// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceMeasure.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_DistanceMeasure::UBTService_DistanceMeasure()
{
    NodeName = TEXT("DistanceMeasure");
}

void UBTService_DistanceMeasure::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if(OwnerComp.GetAIOwner() == nullptr) return;

    AAIController* BossAIController = Cast<AAIController>(OwnerComp.GetAIOwner());

    if(BossAIController == nullptr) return;

    auto BossMonster = Cast<ABossMonster_Stage1>(OwnerComp.GetAIOwner()->GetPawn());
    if(BossMonster == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your Boss null"));
        return;
    }
    auto Target = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Target == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your Target null"));
        return;
    }
    FVector TargetLocation = Target->GetActorLocation();
    float Distance =  FVector::Distance(BossMonster->GetActorLocation(), TargetLocation);
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
}
