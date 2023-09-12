// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PercentBossHP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_PercentBossHP::UBTService_PercentBossHP()
{
    NodeName = TEXT("PercentBossHP");
}

void UBTService_PercentBossHP::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
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

    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), BossMonster->PercentBossHP());

}
