// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BTService.h"
#include "CSGAIPerceptionComponent.generated.h"

class ACSGBaseCharacter;

UCLASS()
class CHEETAH_API UCSGAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	ACSGBaseCharacter* GetEnemy();

	float CalculateEnemyDetectionLevel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyDetectionLevelKey;
};
