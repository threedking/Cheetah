// Copyright Threedking. All Rights Reserved.


#include "BaseCharacter.h"

#include "CalculableLightSource.h"

ABaseCharacter::ABaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

float ABaseCharacter::GetFullLightIllumination()
{
    return FMath::Clamp(GetIlluminationByLightSources() + GetIlluminationByGlobalLighth(), 0.f, 1.f);
}
float ABaseCharacter::GetIlluminationByLightSources()
{
    float IlluminationLevel = 0.f;

    for (ACalculableLightSource* LightSource : LightSources)
    {
        IlluminationLevel += LightSource->GetIlluminationLevel(this);
    }

    return FMath::Clamp(IlluminationLevel, 0.f, 1.f);
}

void ABaseCharacter::AddObservingLightSource(ACalculableLightSource* LightSource)
{
    if (!LightSource) return;

    LightSources.Add(LightSource);
}

void ABaseCharacter::RemoveObservingLightSource(ACalculableLightSource* LightSource)
{
    if (!LightSource) return;

    LightSources.Remove(LightSource);
}

const TArray<FName>& ABaseCharacter::GetLightSocketNames()
{
    return LightSocketNames;
}

TArray<FVector> ABaseCharacter::GetImportantBonesLocation()
{
    TArray<FVector> ImportantBoneLocations;
    
    for (FName LightSocketName : LightSocketNames)
    {
        ImportantBoneLocations.Add(GetMesh()->GetSocketLocation(LightSocketName));
    }

    return ImportantBoneLocations;
}
