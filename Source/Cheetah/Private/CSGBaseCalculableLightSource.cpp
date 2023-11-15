// Copyright Threedking. All Rights Reserved.


#include "CSGBaseCalculableLightSource.h"

#include "Components/SphereComponent.h"
#include "Curves/CurveFloat.h"
#include "CSGBaseCharacter.h"
#include "AI/CSGAICharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACSGBaseCalculableLightSource::ACSGBaseCalculableLightSource()
{
 	PrimaryActorTick.bCanEverTick = true;
    EdgeCollisionComponent = CreateDefaultSubobject<USphereComponent>("EdgeSphereComponent");
    EdgeCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    EdgeCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(EdgeCollisionComponent);

    DamageCollisionComponent = CreateDefaultSubobject<USphereComponent>("DamageSphereComponent");
    DamageCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    DamageCollisionComponent->SetupAttachment(GetRootComponent());

    EdgeCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACSGBaseCalculableLightSource::EdgeBeginOverlap);
    EdgeCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACSGBaseCalculableLightSource::EdgeEndOverlap);
}

void ACSGBaseCalculableLightSource::BeginPlay()
{
	Super::BeginPlay();

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    UClass* SeekClass = ACSGBaseCharacter::StaticClass();
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Init(this, 1);
    TArray<AActor*> FoundActors;

    if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), EdgeCollisionComponent->GetUnscaledSphereRadius(), ObjectTypes, SeekClass, IgnoreActors, FoundActors))
    {
        for (AActor*& FoundActor : FoundActors)
        {
            ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(FoundActor);
            if (BaseCharacter && BaseCharacter->IsPlayer())
            {
                BaseCharacter->AddObservingLightSource(this);
            }
        }
    }	
}

float ACSGBaseCalculableLightSource::GetIlluminationLevel(ACSGBaseCharacter* Target)
{
    if (!GetWorld() || !Target) return 0.f;

    float IlluminationLevel = 0.f; // Resulting value

    float IlluminationLevelByDistance = 0.f;
    float IlluminationLevelByViewAngle = 0.f;
    float IlluminationLevelByViewAngleCoeff = 0.f;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    const TArray<FVector> ImportantBonesLocation = Target->GetImportantBonesLocation();
    /*
    The illumination level consists of the sum of the illumination of the important bones
    Bone is illuminated if the LineTrace from the LightSource reaches it, it's close enough and inside of view cone
    */
    for (FVector ImportantBoneLocation : ImportantBonesLocation)
    {
        if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), ImportantBoneLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
        {
            AActor* HitedActor = HitResult.GetActor();
            if (HitedActor == Target)
            {
                // Angle between LightSource straight direction and direction to the bone
                float Angle = FMath::RadiansToDegrees(
                    FMath::Acos(
                        FVector::DotProduct(
                            GetActorForwardVector(), (HitResult.Location - GetActorLocation()).GetSafeNormal())));
                if (Angle <= ViewAngle / 2.f && HitResult.Distance <= EdgeCollisionComponent->GetUnscaledSphereRadius())
                {
                    //Each bone adds its part to the total illumination level
                    float IlluminationLevelDelta = 1.f / ImportantBonesLocation.Num();
                    
                    /*
                    If IlluminationIsConst is true, distance and angle to the bone doesn't matter 
                    and illumination level of this bone is maximum
                    */                    
                    if (!IlluminationIsConst && ViewAngleToCoeffCurve && DistanceToCoeffCurve &&
                        EdgeCollisionComponent->GetUnscaledSphereRadius() > 0.0f && ViewAngle > 0.0f)
                    {
                        //Calculating the distance coefficient, using curve to change the linear dependence
                        float DistanceCoeff = DistanceToCoeffCurve->GetFloatValue(HitResult.Distance / EdgeCollisionComponent->GetUnscaledSphereRadius());
                        IlluminationLevelDelta *= DistanceCoeff;

                        /*
                        Calculating the angle coefficient, using curve to change the linear dependence
                        The total illumination level by angle gets from best variant
                        Inverse linear dependence on distance is taken as a coefficient for illumination level by angle 
                        */
                        float AngleDistanceCoeff = ViewAngleToCoeffCurve->GetFloatValue(Angle / (ViewAngle / 2.f));
                        if (AngleDistanceCoeff > IlluminationLevelByViewAngle)
                        {
                            IlluminationLevelByViewAngle = AngleDistanceCoeff;
                            IlluminationLevelByViewAngleCoeff = 1.f - DistanceCoeff;
                        }
                    }
                    IlluminationLevelByDistance += IlluminationLevelDelta;

                    if (Target->IsDrawDebug())
                    {
                        DrawDebugLine(GetWorld(), GetActorLocation(), ImportantBoneLocation, FColor::Yellow);
                    }
                }
            }
        }
    }

    /*
    Resulting Illumination Level as arithmetic average between
    product of Illumination by distance and Illumination by view angle with Illumination by distance, taking into account their coefficients
    So illumination level by angle has inverse linear dependence on distance and resulting Illumination has linear dependence on distance
    */
    if (!IlluminationIsConst)
    {
        IlluminationLevel = ((IlluminationLevelByDistance * IlluminationLevelByViewAngle) * IlluminationLevelByViewAngleCoeff + IlluminationLevelByDistance) / (1.f + IlluminationLevelByViewAngleCoeff);
    }

    return FMath::Clamp(IlluminationLevel, 0.f, 1.f);
}

void ACSGBaseCalculableLightSource::EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter || !BaseCharacter->IsPlayer()) return;

    BaseCharacter->AddObservingLightSource(this);
}

void ACSGBaseCalculableLightSource::EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter || !BaseCharacter->IsPlayer()) return;

    BaseCharacter->RemoveObservingLightSource(this);
}
