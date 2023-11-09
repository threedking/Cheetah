// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ACalculableLightSource;

UCLASS()
class CHEETAH_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void AddObservingLightSource(ACalculableLightSource* LightSource);
	void RemoveObservingLightSource(ACalculableLightSource* LightSource);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> LightSocketNames;

	TArray<ACalculableLightSource*> LightSources;
};
