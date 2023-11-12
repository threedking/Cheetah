// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CSGAttackEnemyService.generated.h"

UCLASS()
class CHEETAH_API UCSGAttackEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UCSGAttackEnemyService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
