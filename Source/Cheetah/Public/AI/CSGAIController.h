// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CSGAIController.generated.h"

class UCSGAIPerceptionComponent;

/**
* Mood is main AI state, that define bot behavior (like what to do noticing player or suspicion event)
* Mood can be observed by player as clue of bot's plans
*/
UENUM(BlueprintType)
enum class EAIMood : uint8
{
	Calm,
	Suspicious,
	Angry
};

UCLASS()
class CHEETAH_API ACSGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACSGAIController();

protected:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCSGAIPerceptionComponent> CSGAIPerceptionComponent;

	virtual void OnPossess(APawn* InPawn) override;
};
