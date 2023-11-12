// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CSGAttackEnemyTask.generated.h"

/**
 * 
 */
UCLASS()
class CHEETAH_API UCSGAttackEnemyTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCSGAttackEnemyTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;
};
