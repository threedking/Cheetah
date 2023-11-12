// Copyright Threedking. All Rights Reserved.


#include "AI/Tasks/CSGCalmDownTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"

UCSGCalmDownTask::UCSGCalmDownTask()
{
    NodeName = "Calm Down";
}

EBTNodeResult::Type UCSGCalmDownTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(Controller->GetPawn());
    if (!AICharacter)  return EBTNodeResult::Failed;

    Blackboard->SetValueAsEnum(Mood.SelectedKeyName, static_cast<uint8>(EAIMood::Calm));

    AICharacter->SetIsRun(false);

    return EBTNodeResult::Succeeded;
}
