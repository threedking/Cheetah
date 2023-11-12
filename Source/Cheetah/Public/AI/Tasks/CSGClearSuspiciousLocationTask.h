// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CSGClearSuspiciousLocationTask.generated.h"

UCLASS()
class CHEETAH_API UCSGClearSuspiciousLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UCSGClearSuspiciousLocationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector SuspiciousLocationKey;
};
