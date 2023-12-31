// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGNextPatrolPointTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"


UCSGNextPatrolPointTask::UCSGNextPatrolPointTask()
{
	NodeName = "Next Patrol Point";
}

EBTNodeResult::Type UCSGNextPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(Controller->GetPawn());
    if (!AICharacter)  return EBTNodeResult::Failed;
 
    Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, AICharacter->GetNextPatrolLocation());

    return EBTNodeResult::Succeeded;
}
