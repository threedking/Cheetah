// Copyright Threedking. All Rights Reserved.


#include "AI/Services/CSGDetectEnemyService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CSGAIPerceptionComponent.h"
#include "CSGBaseCharacter.h"
#include "AIController.h"

UCSGDetectEnemyService::UCSGDetectEnemyService()
{
	NodeName = "Detect Enemy";
}

void UCSGDetectEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        AAIController* AIController = OwnerComp.GetAIOwner();
        if (!AIController) return;

        APawn* Pawn = AIController->GetPawn();
        if (!Pawn) return;

        UCSGAIPerceptionComponent* PerceptionComponent = Pawn->FindComponentByClass<UCSGAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            ACSGBaseCharacter* BaseCharacter = PerceptionComponent->GetEnemy();
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, BaseCharacter);
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
