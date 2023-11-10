// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"

ACSGAICharacter::ACSGAICharacter()
{
    AutoPossessAI = EAutoPossessAI::Disabled;

    if (GetCharacterMovement())
    {
        bUseControllerRotationYaw = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }

    PatrolSplineComponent = CreateDefaultSubobject<USplineComponent>("Patrol Spline Component");
}

FVector ACSGAICharacter::GetNextPatrolLocation()
{
    FVector NextPatrolLocation = GetActorLocation();

    const int32 CountPatrolPoints = PatrolSplineComponent->GetNumberOfSplinePoints();
    if (CountPatrolPoints == 0) return NextPatrolLocation;
    if (CountPatrolPoints == 1) return PatrolSplineComponent->GetWorldLocationAtSplinePoint(0);

    if ((IsPatrolingForward && CurrentPatrolPointIndex == CountPatrolPoints - 1) ||
        (!IsPatrolingForward && CurrentPatrolPointIndex == 0))
    {
        if (IsLoopPatrol)
        {
            if (IsPatrolingForward && CurrentPatrolPointIndex == CountPatrolPoints - 1)
            {
                CurrentPatrolPointIndex = -1;
            }
            else if (!IsPatrolingForward && CurrentPatrolPointIndex == 0)
            {
                CurrentPatrolPointIndex = CountPatrolPoints;
            }
        }else{
            IsPatrolingForward = !IsPatrolingForward;
        }
    }

    CurrentPatrolPointIndex += IsPatrolingForward ? 1 : -1;
    
    NextPatrolLocation = PatrolSplineComponent->GetWorldLocationAtSplinePoint(CurrentPatrolPointIndex);

    return NextPatrolLocation;
}
