// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BTService.h"
#include "CSGAIPerceptionComponent.generated.h"

class ACSGBaseCharacter;
struct FActorPerceptionUpdateInfo;

UCLASS()
class CHEETAH_API UCSGAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	float CalculateEnemyDetectionLevel();

	virtual void PostInitProperties() override;

	void ClearAwareness();

	void AddSuspiciousAwareness(float AdditionalAwareness);

	void AddAngryAwareness(float AdditionalAwareness);

	UFUNCTION(BlueprintCallable)
	float GetSuspiciousAwareness();

	UFUNCTION(BlueprintCallable)
	float GetAngryAwareness();

	TObjectPtr<ACSGBaseCharacter> GetEnemy();

	bool GetSuspiciousLocation(FVector& OutSuspiciousLocation);

protected:
	TObjectPtr<ACSGBaseCharacter> Player;

	FVector SuspiciousLocation;
	bool IsHasSuspiciousLocation = false;

	float SuspiciousAwareness = 0.f;
	float AngryAwareness = 0.f;

	UFUNCTION()
	void PerceptionUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
};
