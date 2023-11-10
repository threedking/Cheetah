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
    
    /*
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AICharacter);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;

    FVector Location = AICharacter->GetActorLocation();
        
    bool Found = NavSys->GetRandomReachablePointInRadius(Location, 1000.f, NavLocation); // temporary random
    if (!Found) return EBTNodeResult::Failed;
    */

    Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, AICharacter->GetNextPatrolLocation());

    return EBTNodeResult::Succeeded;
}
