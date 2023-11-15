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
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	/** If true, bot will not move without player influence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	bool IsPatroling = true;

	/** 
	* If true, bot will patrol through the PatrolSplineComponent's points in a loop 
	* If false, bot will patrol through the PatrolSplineComponent's points from edge to edge
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	bool IsLoopPatrol = false;
	
	/** 
	* Current patrol direction
	* It's loop direction if IsLoopPatrol is true
	* It's current patrol direction if IsLoopPatrol is false and will changing automatically on the edges
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	bool IsPatrolingForward = true;

	/** 
	* Return current detection level of visible enemy
	* Depends on distance, illumination, closeness to straight view direction, pose and movement speed
	*/
	UFUNCTION(BlueprintCallable)
	float GetEnemyDetectionLevel(ACSGBaseCharacter* EnemyCharacter);

	/** Try to attack the enemy in capable way */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AttackEnemyActor(AActor* Enemy);

	virtual bool IsPlayer() override;

protected:

	/** Max distance to detect the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	float ViewDistance = 0.f;

	/** Angle of View */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	float ViewAngle = 0.f;

	/** If the bot sees less player's bones than this value, then he doesn't see player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	int32 MinimumBonesRequiredToSee = 0;

	/** If true, Angle Stance and Motion coeffs will be affected by distance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	bool IsAngleStanceMotionDetectionDependsOnDistance = true;

	/** Detection coefficient by angle between straight view direction and direction to the player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByAngle = 1.0f;

	/** Detection coefficient by player's pose (is crouched) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByStance = 1.0f;

	/** Detection coefficient by player's movement speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByMotion = 1.0f;

	/** 
	* Detection coefficient by Angle, Stance and Motion coefficients
	* It determine affect three parameters on whole detection level in relation to detection by distance and illumination
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float CoeffDetectionByAngleStanceMotion = 1.0f;

	/** 
	* Whole Detection level coefficient
	* Can be used as a difficulty level.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Detection")
	float MainCoeffDetection = 1.0f;

	/** Used to change the linear dependence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	TObjectPtr<UCurveFloat> ViewAngleToCoeffCurve;

	/** Used to change the linear dependence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Detection")
	TObjectPtr<UCurveFloat> DistanceToCoeffCurve;

	/** SplineComponent used to store and modify patrol points */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<USplineComponent> PatrolSplineComponent;

	int32 CurrentPatrolPointIndex = 0;

	void GetDetectionByDistanceAndAngle(ACSGBaseCharacter* EnemyCharacter, float& DetectionByDistance, float& DetectionByAngle);
};
