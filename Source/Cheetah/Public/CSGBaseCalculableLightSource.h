// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSGBaseCalculableLightSource.generated.h"

class USphereComponent;
class UCurveFloat;
class ACSGBaseCharacter;

UCLASS()
class CHEETAH_API ACSGBaseCalculableLightSource : public AActor
{
	GENERATED_BODY()
	
public:
	ACSGBaseCalculableLightSource();

	virtual void BeginPlay();

	float GetIlluminationLevel(ACSGBaseCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void SetIsTurnedOn(bool NewState);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> EdgeCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> DamageCollisionComponent;

	/** 
	* If true, this source will Illuminate player only in depend of cout bones are under light 
	* If all player's bones are under light, player gets max Illumination level
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	bool IlluminationIsConst = true;

	/** Used to change the linear dependence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination", meta = (EditCondition = "!IlluminationIsConst"))
	TObjectPtr<UCurveFloat> ViewAngleToCoeffCurve;

	/** Used to change the linear dependence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination", meta = (EditCondition = "!IlluminationIsConst"))
	TObjectPtr<UCurveFloat> DistanceToCoeffCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	bool IsTurnedOn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	float ViewAngle = 0.f;

	UFUNCTION()
	void EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
