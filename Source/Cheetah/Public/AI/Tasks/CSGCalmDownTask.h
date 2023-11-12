// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CSGCalmDownTask.generated.h"

UCLASS()
class CHEETAH_API UCSGCalmDownTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UCSGCalmDownTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector Mood;
};
