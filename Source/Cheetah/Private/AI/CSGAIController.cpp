// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAIController.h"

#include "AI/CSGAICharacter.h"
#include "AI/CSGAIPerceptionComponent.h"

ACSGAIController::ACSGAIController()
{
    CSGAIPerceptionComponent = this->CreateDefaultSubobject<UCSGAIPerceptionComponent>("CSG AI Perception Component");
    SetPerceptionComponent(*CSGAIPerceptionComponent);
}

void ACSGAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(InPawn);
    if (AICharacter && AICharacter->BehaviorTreeAsset)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }

}
