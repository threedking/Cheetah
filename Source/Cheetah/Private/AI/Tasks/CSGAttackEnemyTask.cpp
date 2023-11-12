// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGAttackEnemyTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"

UCSGAttackEnemyTask::UCSGAttackEnemyTask()
{
	NodeName = "Attack Enemy";
}

EBTNodeResult::Type UCSGAttackEnemyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(Controller->GetPawn());
    if (!AICharacter)  return EBTNodeResult::Failed;

    ACSGBaseCharacter* EnemyCharacter = Cast<ACSGBaseCharacter>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!EnemyCharacter) return EBTNodeResult::Failed;
    
    AICharacter->AttackEnemyActor(EnemyCharacter);

    return EBTNodeResult::Succeeded;
}
