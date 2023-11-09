// Copyright Threedking. All Rights Reserved.


#include "CalculableLightSource.h"

#include "Components/SphereComponent.h"
#include "Curves/CurveFloat.h"
#include "BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACalculableLightSource::ACalculableLightSource()
{
	PrimaryActorTick.bCanEverTick = true;

    EdgeCollisionComponent = CreateDefaultSubobject<USphereComponent>("EdgeSphereComponent");
    EdgeCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    EdgeCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(EdgeCollisionComponent);

    EdgeCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACalculableLightSource::EdgeBeginOverlap);
    EdgeCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACalculableLightSource::EdgeEndOverlap);
}

void ACalculableLightSource::BeginPlay()
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    UClass* SeekClass = ABaseCharacter::StaticClass();
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Init(this, 1);

    TArray<AActor*> FoundActors;

    if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), EdgeCollisionComponent->GetUnscaledSphereRadius(), ObjectTypes, SeekClass, IgnoreActors, FoundActors))
    {
        for (AActor*& FoundActor : FoundActors)
        {
            ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(FoundActor);
            if (BaseCharacter)
            {
                BaseCharacter->AddObservingLightSource(this);
            }
        }
    }
}

float ACalculableLightSource::GetIlluminationLevel(ABaseCharacter* Target)
{
    if (!GetWorld()) return 0.f;

    ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Target);
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

/*void ACalculableLightSource::NotifyActorBeginOverlap(AActor* OtherActor)
{
    ABaseCharacter* BCharacter = Cast<ABaseCharacter>(OtherActor);
    if (!BCharacter) return;

    BCharacter->AddObservingLightSource(this);
}

void ACalculableLightSource::NotifyActorEndOverlap(AActor* OtherActor)
{
    ABaseCharacter* BCharacter = Cast<ABaseCharacter>(OtherActor);
    if (!BCharacter) return;

    BCharacter->RemoveObservingLightSource(this);
}*/

void ACalculableLightSource::EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    BaseCharacter->AddObservingLightSource(this);
}

void ACalculableLightSource::EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
    if (!BaseCharacter) return;

    BaseCharacter->RemoveObservingLightSource(this);
}
