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
	
	virtual void BeginPlay();

	void AddObservingLightSource(ACSGBaseCalculableLightSource* LightSource);
	void RemoveObservingLightSource(ACSGBaseCalculableLightSource* LightSource);

	/** Calculate and get Illumination level by Directional Light */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetIlluminationByGlobalLighth();

	/** Calculate and get Illumination level by all ACSGBaseCalculableLightSource objects from LightSources array*/
	UFUNCTION(BlueprintCallable)
	float GetIlluminationByLightSources();

	/** Calculate and get Illumination level by Directional Light and all LightSources */
	UFUNCTION(BlueprintCallable)
	float GetFullLightIllumination();

	/** Socket names of important bones for illumination calculation */
	UFUNCTION(BlueprintCallable)
	const TArray<FName>& GetLightSocketNames();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetImportantBonesLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsDrawDebug();

	UFUNCTION(BlueprintCallable)
	void SetIsRun(bool NewIsRun);

	UFUNCTION(BlueprintCallable)
	bool GetIsRun();

	UFUNCTION(BlueprintCallable)
	virtual bool IsPlayer();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInCover = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsRun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRunSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> LightSocketNames;

	TArray<ACSGBaseCalculableLightSource*> LightSources;

};
