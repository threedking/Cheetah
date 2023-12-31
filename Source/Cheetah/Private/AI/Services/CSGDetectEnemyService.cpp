// Copyright Threedking. All Rights Reserved.


#include "AI/Services/CSGDetectEnemyService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CSGAIPerceptionComponent.h"
#include "CSGBaseCharacter.h"
#include "AIController.h"
#include "AI/CSGAIController.h"
#include "AI/CSGAICharacter.h"

UCSGDetectEnemyService::UCSGDetectEnemyService()
{
	NodeName = "Detect Enemy";
}

void UCSGDetectEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    UCSGAIPerceptionComponent* PerceptionComponent = AIController->FindComponentByClass<UCSGAIPerceptionComponent>();
    if (!PerceptionComponent) return;
    
    uint8 Moodu8 = Blackboard->GetValueAsEnum(Mood.SelectedKeyName);
    EAIMood MoodEnum = static_cast<EAIMood>(Moodu8);

    float EnemyDetectionLevel = PerceptionComponent->CalculateEnemyDetectionLevel();
    bool IsSeeEnemy = !FMath::IsNearlyZero(EnemyDetectionLevel);

    /*
    Go to suspicious mood if PerceptionComponent detected suspicious activity(heard something), 
    but only if we are not angry(attacking or searching enemy)
    */
    FVector SuspiciousLocation = FVector::Zero();
    if (PerceptionComponent->GetSuspiciousLocation(SuspiciousLocation))
    {
        if (MoodEnum != EAIMood::Angry)
        {
            Blackboard->SetValueAsEnum(Mood.SelectedKeyName, static_cast<uint8>(EAIMood::Suspicious));
        }
        Blackboard->SetValueAsVector(Suspiciouslocation.SelectedKeyName, SuspiciousLocation);
    }

    if (!IsSeeEnemy)
    {
        ACSGBaseCharacter* SuspiciousCharacter = Cast<ACSGBaseCharacter>(Blackboard->GetValueAsObject(SuspiciousActor.SelectedKeyName));
        ACSGBaseCharacter* EnemyCharacter = Cast<ACSGBaseCharacter>(Blackboard->GetValueAsObject(EnemyActor.SelectedKeyName));

        /*
        If we don't see the enemy now, but have his value in Blackboard, 
        then we have just lost sight of him and have to notice his last location as suspicious
        */
        if ((MoodEnum == EAIMood::Suspicious && SuspiciousCharacter) ||
            (MoodEnum == EAIMood::Angry && EnemyCharacter))
        {
            Blackboard->SetValueAsVector(Suspiciouslocation.SelectedKeyName, EnemyCharacter ? EnemyCharacter->GetActorLocation() : SuspiciousCharacter->GetActorLocation());
        }
        Blackboard->ClearValue(SuspiciousActor.SelectedKeyName);
        Blackboard->ClearValue(EnemyActor.SelectedKeyName);
        PerceptionComponent->ClearAwareness();
        return;
    }

    ACSGBaseCharacter* Player = PerceptionComponent->GetEnemy();
    if (!Player) return;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(AIController->GetPawn());
    if (!AICharacter) return;

    /*
    Adding a EnemyDetectionLevel to specific awareness.
    We change the mood when awareness reach 1, depending on the current mood and notify AI about the player
    */
    switch (MoodEnum)
    {
    case EAIMood::Calm:
        PerceptionComponent->AddSuspiciousAwareness(EnemyDetectionLevel);
        if (FMath::IsNearlyEqual(PerceptionComponent->GetSuspiciousAwareness(), 1.f))
        {
            Blackboard->SetValueAsEnum(Mood.SelectedKeyName, static_cast<uint8>(EAIMood::Suspicious));
            Blackboard->SetValueAsObject(SuspiciousActor.SelectedKeyName, Player);
            AICharacter->SetIsRun(false);
        }
        break;
    case EAIMood::Suspicious:
        PerceptionComponent->AddAngryAwareness(EnemyDetectionLevel);
        if (FMath::IsNearlyEqual(PerceptionComponent->GetAngryAwareness(), 1.f))
        {
            Blackboard->SetValueAsEnum(Mood.SelectedKeyName, static_cast<uint8>(EAIMood::Angry));
            Blackboard->SetValueAsObject(EnemyActor.SelectedKeyName, Player);
            AICharacter->SetIsRun(true);
        }else{
            Blackboard->SetValueAsEnum(Mood.SelectedKeyName, static_cast<uint8>(EAIMood::Suspicious));
            Blackboard->SetValueAsObject(SuspiciousActor.SelectedKeyName, Player);
            AICharacter->SetIsRun(false);
        }
        break;
    case EAIMood::Angry:
        Blackboard->SetValueAsObject(EnemyActor.SelectedKeyName, Player);
        break;
    }
}
