// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CSGBaseCharacter.h"
#include "CSGAICharacter.generated.h"

class UBehaviorTree;
class USplineComponent;
class UCurveFloat;

UCLASS()
class CHEETAH_API ACSGAICharacter : public ACSGBaseCharacter
{
	GENERATED_BODY()

public:
	ACSGAICharacter();

	FVector GetNextPatrolLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Patrol")
	bool IsPatroling = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Patrol")
	bool IsLoopPatrol = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Patrol")
	bool IsPatrolingForward = true;

	UFUNCTION(BlueprintCallable)
	float GetEnemyDetectionLevel(ACSGBaseCharacter* EnemyCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AttackEnemyActor(AActor* Enemy);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	float ViewDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	float ViewAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	int32 MinimumBonesRequiredToSee = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	bool IsAngleStanceMotionDetectionDependsOnDistance = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByAngle = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByStance = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByMotion = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByAngleStanceMotion = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float MainCoeffDetection = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	UCurveFloat* ViewAngleToCoeffCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	UCurveFloat* DistanceToCoeffCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	USplineComponent* PatrolSplineComponent;

	int32 CurrentPatrolPointIndex = 0;

	void GetDetectionByDistanceAndAngle(ACSGBaseCharacter* EnemyCharacter, float& DetectionByDistance, float& DetectionByAngle);
};
