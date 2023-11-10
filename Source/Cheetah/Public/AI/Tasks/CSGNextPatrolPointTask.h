// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CSGNextPatrolPointTask.generated.h"

/**
 * 
 */
UCLASS()
class CHEETAH_API UCSGNextPatrolPointTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UCSGNextPatrolPointTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector LocationKey;
};
