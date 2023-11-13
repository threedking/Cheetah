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
            ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(FoundActor);
            if (BaseCharacter && !AICharacter)
            {
                BaseCharacter->AddObservingLightSource(this);
            }
        }
    }	
}

float ACSGBaseCalculableLightSource::GetIlluminationLevel(ACSGBaseCharacter* Target)
{
    if (!GetWorld()) return 0.f;

    float IlluminationLevel = 0.f;

    float IlluminationLevelByViewAngle = 0.f;
    float IlluminationLevelByViewAngleCoeff = 0.f;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    const TArray<FVector> ImportantBonesLocation = Target->GetImportantBonesLocation();

    for (FVector ImportantBoneLocation : ImportantBonesLocation)
    {
        if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), ImportantBoneLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
        {
            AActor* HitedActor = HitResult.GetActor();
            if (HitedActor == Target)
            {
                float Angle = FMath::RadiansToDegrees(
                    FMath::Acos(
                        FVector::DotProduct(
                            GetActorForwardVector(), (HitResult.Location - GetActorLocation()).GetSafeNormal())));
                if (Angle <= ViewAngle / 2.f && HitResult.Distance <= EdgeCollisionComponent->GetUnscaledSphereRadius())
                {
                    float IlluminationLevelDelta = 1.f / ImportantBonesLocation.Num();
                    
                    if (!IlluminationIsConst && ViewAngleToCoeffCurve && DistanceToCoeffCurve &&
                        EdgeCollisionComponent->GetUnscaledSphereRadius() > 0.0f && ViewAngle > 0.0f)
                    {
                        float DistanceCoeff = DistanceToCoeffCurve->GetFloatValue(HitResult.Distance / EdgeCollisionComponent->GetUnscaledSphereRadius());
                        IlluminationLevelDelta *= DistanceCoeff;

                        float AngleDistanceCoeff = ViewAngleToCoeffCurve->GetFloatValue(Angle / (ViewAngle / 2.f));
                        if (AngleDistanceCoeff > IlluminationLevelByViewAngle)
                        {
                            IlluminationLevelByViewAngle = AngleDistanceCoeff;
                            IlluminationLevelByViewAngleCoeff = 1.f - DistanceCoeff;
                        }
                    }
                    IlluminationLevel += IlluminationLevelDelta;

                    if (Target->IsDrawDebug())
                    {
                        DrawDebugLine(GetWorld(), GetActorLocation(), ImportantBoneLocation, FColor::Yellow);
                    }
                }
            }
        }
    }

    if (!IlluminationIsConst)
    {
        IlluminationLevel = ((IlluminationLevel * IlluminationLevelByViewAngle) * IlluminationLevelByViewAngleCoeff + IlluminationLevel) / (1.f + IlluminationLevelByViewAngleCoeff);
    }

    return FMath::Clamp(IlluminationLevel, 0.f, 1.f);
}

void ACSGBaseCalculableLightSource::EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(OtherActor);
    if (AICharacter) return;

    BaseCharacter->AddObservingLightSource(this);
}

void ACSGBaseCalculableLightSource::EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    ACSGAICharacter* AICharacter = Cast<ACSGAICharacter>(OtherActor);
    if (AICharacter) return;

    BaseCharacter->RemoveObservingLightSource(this);
}
