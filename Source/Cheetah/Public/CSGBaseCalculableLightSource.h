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
	USphereComponent* EdgeCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	bool IlluminationIsConst = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination", meta = (EditCondition = "!IlluminationIsConst"))
	UCurveFloat* ViewAngleToCoeffCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination", meta = (EditCondition = "!IlluminationIsConst"))
	UCurveFloat* DistanceToCoeffCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	bool IsTurnedOn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illumination")
	float ViewAngle = 0.f;

	UFUNCTION()
	void EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
