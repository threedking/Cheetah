// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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

bool ACSGAICharacter::IsPlayer()
{
    return false;
}

FVector ACSGAICharacter::GetNextPatrolLocation()
{
    FVector NextPatrolLocation = GetActorLocation();

    if (!IsPatroling) return NextPatrolLocation;

    // SplineComponent used to store and modify patrol points
    const int32 CountPatrolPoints = PatrolSplineComponent->GetNumberOfSplinePoints();
    if (CountPatrolPoints == 0) return NextPatrolLocation;
    if (CountPatrolPoints == 1) return PatrolSplineComponent->GetWorldLocationAtSplinePoint(0);

    // Have we reached the edge of patrol path?
    if ((IsPatrolingForward && CurrentPatrolPointIndex == CountPatrolPoints - 1) ||
        (!IsPatrolingForward && CurrentPatrolPointIndex == 0))
    {
        if (IsLoopPatrol)
        {
            if (IsPatrolingForward && CurrentPatrolPointIndex == CountPatrolPoints - 1)
            {
                CurrentPatrolPointIndex = -1; // Will be increased after if statement block
            }
            else if (!IsPatrolingForward && CurrentPatrolPointIndex == 0)
            {
                CurrentPatrolPointIndex = CountPatrolPoints; // Will be decreased after if statement block
            }
        }else{
            IsPatrolingForward = !IsPatrolingForward;
        }
    }

    CurrentPatrolPointIndex += IsPatrolingForward ? 1 : -1;
    
    NextPatrolLocation = PatrolSplineComponent->GetWorldLocationAtSplinePoint(CurrentPatrolPointIndex);

    return NextPatrolLocation;
}

float ACSGAICharacter::GetEnemyDetectionLevel(ACSGBaseCharacter* EnemyCharacter)
{
    if (!EnemyCharacter) return 0.f;

    UPawnMovementComponent* EnemyMovementComponent = EnemyCharacter->GetMovementComponent();
    if (!EnemyMovementComponent) return 0.f;

    float DetectionLevel = 0.f; // Resulting value

    float DetectionByIllumination = EnemyCharacter->GetFullLightIllumination();
    float DetectionByDistance = 0.f;
    float DetectionByAngle = 0.f; // Angle between bot's straight view direction and direction to the player
    GetDetectionByDistanceAndAngle(EnemyCharacter, DetectionByDistance, DetectionByAngle);

    if (FMath::IsNearlyZero(DetectionByIllumination) || FMath::IsNearlyZero(DetectionByDistance)) return 0.f;

    float DetectionByStance = EnemyCharacter->bIsCrouched ? 1.f : 0.f;
    float DetectionByMotion = FMath::IsNearlyZero(EnemyMovementComponent->GetMaxSpeed()) ? 0.f : 
        FMath::Clamp(EnemyMovementComponent->Velocity.Size() / EnemyMovementComponent->GetMaxSpeed(), 0.f, 1.f);
    
    // Detection by three parameters as arithmetic average between them, taking into account their coefficients
    float DetectionByAngleStanceMotion = FMath::IsNearlyZero(CoeffDetectionByAngle + CoeffDetectionByStance + CoeffDetectionByMotion) ? 0.f :
        FMath::Clamp((DetectionByAngle * CoeffDetectionByAngle +
            DetectionByStance * CoeffDetectionByStance +
            DetectionByMotion * CoeffDetectionByMotion) /
        (CoeffDetectionByAngle + CoeffDetectionByStance + CoeffDetectionByMotion), 0.f , 1.f);

    // Inverse linear dependence on distance if needed
    if (IsAngleStanceMotionDetectionDependsOnDistance)
    {
        DetectionByAngleStanceMotion *= 1.f - DetectionByDistance;
    }

    /*
    Resulting Detection as arithmetic average between 
    product of Illumination and Distance levels with final motion level, 
    taking into account their coefficients
    */
    DetectionLevel = (DetectionByIllumination * DetectionByDistance + DetectionByAngleStanceMotion * CoeffDetectionByAngleStanceMotion) / (1.f + CoeffDetectionByAngleStanceMotion);

    DetectionLevel *= MainCoeffDetection;

    if (EnemyCharacter->IsDrawDebug())
    {
        UE_LOG(LogTemp, Log, TEXT("AICharacter %s Detection Level is: %f"), *GetName(), DetectionLevel);
    }

    return FMath::Clamp(DetectionLevel, 0.f, 1.f);
}

void ACSGAICharacter::GetDetectionByDistanceAndAngle(ACSGBaseCharacter* EnemyCharacter, float& DetectionByDistance, float& DetectionByAngle)
{
    if (!GetWorld()) return;
    if (!EnemyCharacter) return;
    if (!GetMesh()) return;

    if (!ViewAngleToCoeffCurve || !DistanceToCoeffCurve) return;
    if (FMath::IsNearlyZero(ViewDistance) || FMath::IsNearlyZero(ViewAngle)) return;

    FVector MyHeadLocation = GetMesh()->GetSocketLocation(FName(TEXT("Head")));

    DetectionByDistance = 0.f;
    DetectionByAngle = 0.f;
    int32 SumBonesSeen = 0;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    const TArray<FVector> ImportantBonesLocation = EnemyCharacter->GetImportantBonesLocation();
    /*
    Check the visibility of each important bone
    Bone is visible if the LineTrace from the bot's head reaches it, it's close enough and inside of view cone
    */
    for (FVector ImportantBoneLocation : ImportantBonesLocation)
    {
        if (GetWorld()->LineTraceSingleByChannel(HitResult, MyHeadLocation, ImportantBoneLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
        {
            if (EnemyCharacter == HitResult.GetActor())
            {
                // Angle between bot's straight view direction and direction to the bone
                float Angle = FMath::RadiansToDegrees(
                    FMath::Acos(
                        FVector::DotProduct(
                            GetActorForwardVector(), (HitResult.Location - MyHeadLocation).GetSafeNormal())));
                if (Angle <= ViewAngle / 2.f && HitResult.Distance <= ViewDistance)
                {
                    /*
                    Each bone adds its part to the total detection by distance
                    Calculating the distance coefficient, using curve to change the linear dependence
                    */
                    float DetectionByDistanceDelta = 1.f / ImportantBonesLocation.Num();
                    DetectionByDistanceDelta *= DistanceToCoeffCurve->GetFloatValue(HitResult.Distance / ViewDistance);
                    DetectionByDistance += DetectionByDistanceDelta;

                    /*
                    Calculating the angle coefficient, using curve to change the linear dependence
                    The total detection by angle gets from best variant
                    */
                    float DetectionByAngleCurrent = ViewAngleToCoeffCurve->GetFloatValue(Angle / (ViewAngle / 2.f));
                    if (DetectionByAngleCurrent > DetectionByAngle)
                    {
                        DetectionByAngle = DetectionByAngleCurrent;
                    }

                    SumBonesSeen++;

                    if (EnemyCharacter->IsDrawDebug())
                    {
                        DrawDebugLine(GetWorld(), MyHeadLocation, ImportantBoneLocation, FColor::Red, false, 0.1f);
                    }
                }
            }
        }
    }

    if (SumBonesSeen < MinimumBonesRequiredToSee)
    {
        DetectionByDistance = 0.f;
        DetectionByAngle = 0.f;
    }else {
        DetectionByDistance = FMath::Clamp(DetectionByDistance, 0.f, 1.f);
        DetectionByAngle = FMath::Clamp(DetectionByAngle, 0.f, 1.f);
    }
}
