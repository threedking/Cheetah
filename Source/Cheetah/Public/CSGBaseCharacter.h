// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSGBaseCharacter.generated.h"

class ACSGBaseCalculableLightSource;

UCLASS()
class CHEETAH_API ACSGBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACSGBaseCharacter();

	void AddObservingLightSource(ACSGBaseCalculableLightSource* LightSource);
	void RemoveObservingLightSource(ACSGBaseCalculableLightSource* LightSource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetIlluminationByGlobalLighth();

	UFUNCTION(BlueprintCallable)
	float GetIlluminationByLightSources();

	UFUNCTION(BlueprintCallable)
	float GetFullLightIllumination();

	UFUNCTION(BlueprintCallable)
	const TArray<FName>& GetLightSocketNames();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetImportantBonesLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsDrawDebug();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInCover = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> LightSocketNames;

	TArray<ACSGBaseCalculableLightSource*> LightSources;

};
