// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGNextPatrolPointTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"


UCSGNextPatrolPointTask::UCSGNextPatrolPointTask()
{
	NodeName = "Next Patrol Point";
}

EBTNodeResult::Type UCSGNextPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    APawn* Pawn = Controller->GetPawn();
    if (!Pawn)  return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;

    FVector Location = Pawn->GetActorLocation();
        
    bool Found = NavSys->GetRandomReachablePointInRadius(Location, 1000.f, NavLocation); // temporary random
    if (!Found) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);

    return EBTNodeResult::Succeeded;
}
