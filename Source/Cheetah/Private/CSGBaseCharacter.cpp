// Copyright Threedking. All Rights Reserved.


#include "CSGBaseCharacter.h"

#include "CSGBaseCalculableLightSource.h"

ACSGBaseCharacter::ACSGBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

float ACSGBaseCharacter::GetFullLightIllumination()
{
    return IsInCover ? 0.f : FMath::Clamp(GetIlluminationByLightSources() + GetIlluminationByGlobalLighth(), 0.f, 1.f);
}
float ACSGBaseCharacter::GetIlluminationByLightSources()
{
    float IlluminationLevel = 0.f;

    for (ACSGBaseCalculableLightSource* LightSource : LightSources)
    {
        IlluminationLevel += LightSource->GetIlluminationLevel(this);
    }

    return IsInCover ? 0.f : FMath::Clamp(IlluminationLevel, 0.f, 1.f);
}

void ACSGBaseCharacter::AddObservingLightSource(ACSGBaseCalculableLightSource* LightSource)
{
    if (!LightSource) return;

    LightSources.Add(LightSource);
}

void ACSGBaseCharacter::RemoveObservingLightSource(ACSGBaseCalculableLightSource* LightSource)
{
    if (!LightSource) return;

    LightSources.Remove(LightSource);
}

const TArray<FName>& ACSGBaseCharacter::GetLightSocketNames()
{
    return LightSocketNames;
}

TArray<FVector> ACSGBaseCharacter::GetImportantBonesLocation()
{
    TArray<FVector> ImportantBoneLocations;

    for (FName LightSocketName : LightSocketNames)
    {
        ImportantBoneLocations.Add(GetMesh()->GetSocketLocation(LightSocketName));
    }

    return ImportantBoneLocations;
}
