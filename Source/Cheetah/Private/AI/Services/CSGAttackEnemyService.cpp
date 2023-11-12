// Copyright Threedking. All Rights Reserved.


#include "AI/Services/CSGAttackEnemyService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"

UCSGAttackEnemyService::UCSGAttackEnemyService()
{
    NodeName = "Attack Enemy";
}

void UCSGAttackEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(Controller->GetPawn());
    if (!AICharacter) return;

    ACSGBaseCharacter* EnemyCharacter = Cast<ACSGBaseCharacter>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!EnemyCharacter) return;

    AICharacter->AttackEnemyActor(EnemyCharacter);
}
