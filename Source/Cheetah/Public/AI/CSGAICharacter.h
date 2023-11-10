// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CSGBaseCharacter.h"
#include "CSGAICharacter.generated.h"

class UBehaviorTree;
class USplineComponent;

UCLASS()
class CHEETAH_API ACSGAICharacter : public ACSGBaseCharacter
{
	GENERATED_BODY()

public:
	ACSGAICharacter();

	FVector GetNextPatrolLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	bool IsLoopPatrol = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	bool IsPatrolingForward = true;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	USplineComponent* PatrolSplineComponent;

	int32 CurrentPatrolPointIndex = 0;
};
