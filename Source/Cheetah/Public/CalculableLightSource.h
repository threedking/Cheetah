// Copyright Threedking. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CalculableLightSource.generated.h"

class USphereComponent;
class UCurveFloat;
class ABaseCharacter;

UCLASS()
class CHEETAH_API ACalculableLightSource : public AActor
{
	GENERATED_BODY()
	
public:	
	ACalculableLightSource();

	virtual void BeginPlay() override;

	float GetIlluminationLevel(ABaseCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void SetIsTurnedOn(bool NewState);

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
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

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void EdgeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void EdgeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
