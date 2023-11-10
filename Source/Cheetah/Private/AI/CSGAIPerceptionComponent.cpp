// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAIPerceptionComponent.h"

#include "CSGBaseCharacter.h"
#include "Perception/AISense_Sight.h"

ACSGBaseCharacter* UCSGAIPerceptionComponent::GetEnemy()
{
    TArray<AActor*> PerceptionActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceptionActors);
    if (PerceptionActors.Num() == 0) return nullptr;

    for (AActor* PerceptionActor : PerceptionActors) //There are only 1 now
    {                
        ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(PerceptionActor);
        if (!BaseCharacter) continue;

        return BaseCharacter;
    }

    return nullptr;
}

float UCSGAIPerceptionComponent::CalculateEnemyDetectionLevel()
{
    return 0.1f;
}
