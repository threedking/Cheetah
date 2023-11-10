// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CSGAIController.generated.h"

class UCSGAIPerceptionComponent;

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
	UCSGAIPerceptionComponent* CSGAIPerceptionComponent;

	virtual void OnPossess(APawn* InPawn) override;
};
