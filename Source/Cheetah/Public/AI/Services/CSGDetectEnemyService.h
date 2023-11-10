// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CSGDetectEnemyService.generated.h"

UCLASS()
class CHEETAH_API UCSGDetectEnemyService : public UBTService
{
	GENERATED_BODY()

public:
    UCSGDetectEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
