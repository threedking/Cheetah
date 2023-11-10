// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CSGBaseCharacter.h"
#include "CSGAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class CHEETAH_API ACSGAICharacter : public ACSGBaseCharacter
{
	GENERATED_BODY()

public:
	ACSGAICharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
};
