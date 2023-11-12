// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGFindPointAroundEnemyTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"

UCSGFindPointAroundEnemyTask::UCSGFindPointAroundEnemyTask()
{
	NodeName = "Find Point Around Enemy";
}

EBTNodeResult::Type UCSGFindPointAroundEnemyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(Controller->GetPawn());
    if (!AICharacter)  return EBTNodeResult::Failed;

    ACSGBaseCharacter* EnemyCharacter = Cast<ACSGBaseCharacter>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!EnemyCharacter) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AICharacter);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;

    bool Found = NavSys->GetRandomReachablePointInRadius(EnemyCharacter->GetActorLocation(), 500.f, NavLocation);
    if (!Found) return EBTNodeResult::Failed;    

    Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation);

    return EBTNodeResult::Succeeded;
}
