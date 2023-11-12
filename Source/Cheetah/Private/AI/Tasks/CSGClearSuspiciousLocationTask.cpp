// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGClearSuspiciousLocationTask.h"

#include "BehaviorTree/BlackboardComponent.h"

UCSGClearSuspiciousLocationTask::UCSGClearSuspiciousLocationTask()
{
    NodeName = "Clear Suspicious Location";
}

EBTNodeResult::Type UCSGClearSuspiciousLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    Blackboard->ClearValue(SuspiciousLocationKey.SelectedKeyName);

    return EBTNodeResult::Succeeded;
}
