// Copyright Threedking. All Rights Reserved.


#include "CSGBaseCalculableLightSource.h"

#include "Components/SphereComponent.h"
#include "Curves/CurveFloat.h"
#include "CSGBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACSGBaseCalculableLightSource::ACSGBaseCalculableLightSource()
{
 	PrimaryActorTick.bCanEverTick = true;

    EdgeCollisionComponent = CreateDefaultSubobject<USphereComponent>("EdgeSphereComponent");
    EdgeCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    EdgeCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(EdgeCollisionComponent);

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
            if (BaseCharacter)
            {
                BaseCharacter->AddObservingLightSource(this);
            }
        }
    }	
}

float ACSGBaseCalculableLightSource::GetIlluminationLevel(ACSGBaseCharacter* Target)
{
    if (!GetWorld()) return 0.f;

    ACSGBaseCharacter* TargetCharacter = Cast<ACSGBaseCharacter>(Target);
    if (!TargetCharacter) return 0.f;

    float IlluminationLevel = 0.f;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    const TArray<FVector> ImportantBonesLocation = TargetCharacter->GetImportantBonesLocation();

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
                if (Angle <= ViewAngle / 2.f)
                {
                    float IlluminationLevelDelta = 1.f / ImportantBonesLocation.Num();
                    if (!IlluminationIsConst && ViewAngleToCoeffCurve && DistanceToCoeffCurve)
                    {
                        IlluminationLevelDelta *= ViewAngleToCoeffCurve->GetFloatValue(Angle) * DistanceToCoeffCurve->GetFloatValue(HitResult.Distance);
                    }
                    IlluminationLevel += IlluminationLevelDelta;

                    if (TargetCharacter->IsDrawDebug())
                    {
                        DrawDebugLine(GetWorld(), GetActorLocation(), ImportantBoneLocation, FColor::Yellow);
                    }
                }
            }
        }
    }

    return FMath::Clamp(IlluminationLevel, 0.f, 1.f);
}

void ACSGBaseCalculableLightSource::EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    BaseCharacter->AddObservingLightSource(this);
}

void ACSGBaseCalculableLightSource::EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACSGBaseCharacter* BaseCharacter = Cast<ACSGBaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    BaseCharacter->RemoveObservingLightSource(this);
}
