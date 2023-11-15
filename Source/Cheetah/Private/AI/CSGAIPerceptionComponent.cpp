// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAIPerceptionComponent.h"

#include "CSGBaseCharacter.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/CSGAICharacter.h"
#include "AI/CSGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCSGAIPerceptionComponent::PostInitProperties()
{
    Super::PostInitProperties();

    OnTargetPerceptionInfoUpdated.AddDynamic(this, &UCSGAIPerceptionComponent::PerceptionUpdated);
}

TObjectPtr<ACSGBaseCharacter> UCSGAIPerceptionComponent::GetEnemy()
{
    return Player;
}

void UCSGAIPerceptionComponent::PerceptionUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    if (UpdateInfo.Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
    {
        if (!UpdateInfo.Target.IsValid()) return;

        ACSGBaseCharacter* Character = Cast<ACSGBaseCharacter>(UpdateInfo.Target.Get());
        if (!Character || !Character->IsPlayer()) return;

        //PerceptionComponent just register Player in view cone and not responsible for detection
        if (UpdateInfo.Stimulus.WasSuccessfullySensed())
        {
            if (!Player)
            {
                Player = Character;
            }
        }
        else
        {
            if (Player)
            {
                Player = nullptr;
            }
        }
    }
    else if (UpdateInfo.Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
    {
        if (UpdateInfo.Stimulus.WasSuccessfullySensed())
        {
            SuspiciousLocation = UpdateInfo.Stimulus.StimulusLocation;
            IsHasSuspiciousLocation = true;
        }
    }
}

bool UCSGAIPerceptionComponent::GetSuspiciousLocation(FVector& OutSuspiciousLocation)
{
    if (IsHasSuspiciousLocation)
    {
        OutSuspiciousLocation = SuspiciousLocation;
        IsHasSuspiciousLocation = false;
        return true;
    }

    return false;
}

float UCSGAIPerceptionComponent::CalculateEnemyDetectionLevel()
{
    if (!Player) return 0.f;

    ACSGAIController* MyAIController = Cast<ACSGAIController>(GetOwner());
    if (!MyAIController) return 0.f;

    ACSGAICharacter* MyAICharacter = Cast<ACSGAICharacter>(MyAIController->GetPawn());
    if (!MyAICharacter) return 0.f;

    return MyAICharacter->GetEnemyDetectionLevel(Player);
}

void UCSGAIPerceptionComponent::ClearAwareness()
{
    SuspiciousAwareness = 0.f;
    AngryAwareness = 0.f;
}

void UCSGAIPerceptionComponent::AddSuspiciousAwareness(float AdditionalAwareness)
{
    SuspiciousAwareness = FMath::Clamp(SuspiciousAwareness + AdditionalAwareness, 0.f, 1.f);
}

void UCSGAIPerceptionComponent::AddAngryAwareness(float AdditionalAwareness)
{
    AngryAwareness = FMath::Clamp(AngryAwareness + AdditionalAwareness, 0.f, 1.f);
}

float UCSGAIPerceptionComponent::GetSuspiciousAwareness()
{
    return SuspiciousAwareness;
}

float UCSGAIPerceptionComponent::GetAngryAwareness()
{
    return AngryAwareness;
}
