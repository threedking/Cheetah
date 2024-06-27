// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IlluminationAnalyzerComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnIlluminationCalculated, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHEETAH_API UIlluminationAnalyzerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIlluminationAnalyzerComponent();

	float GetIlluminationLevel();

protected:
	virtual void BeginPlay() override;

	void UpdateIlluminationLevel(float NewIlluminationLevel);

	FOnIlluminationCalculated OnIlluminationCalculated;

	mutable FCriticalSection Mutex;

	float IlluminationLevel = 0.f;

	void CalculateIlluminationLevelAsync();
};
