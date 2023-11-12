// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CSGFindPointAroundEnemyTask.generated.h"

UCLASS()
class CHEETAH_API UCSGFindPointAroundEnemyTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UCSGFindPointAroundEnemyTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector LocationKey;
};
