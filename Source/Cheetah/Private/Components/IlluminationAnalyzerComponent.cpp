// Copyright Threedking. All Rights Reserved.


#include "Components/IlluminationAnalyzerComponent.h"

#include "Engine/Engine.h"
#include "CSGBaseCharacter.h"
//#include "Unix/UnixCriticalSection.h"

UIlluminationAnalyzerComponent::UIlluminationAnalyzerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIlluminationAnalyzerComponent::BeginPlay()
{
	Super::BeginPlay();

	OnIlluminationCalculated.BindUObject(this, &UIlluminationAnalyzerComponent::UpdateIlluminationLevel);

	CalculateIlluminationLevelAsync();
}

void UIlluminationAnalyzerComponent::UpdateIlluminationLevel(float NewIlluminationLevel)
{
	IlluminationLevel = NewIlluminationLevel;
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, FString::Printf(TEXT("IlluminationLevel: %s"), *FString::SanitizeFloat(NewIlluminationLevel)));
}

void UIlluminationAnalyzerComponent::CalculateIlluminationLevelAsync()
{

	AsyncTask(ENamedThreads::AnyThread, [this]()
		{
			float NewIlluminationLevel = 0;
			bool Locked;
			while (GEngine)
			{
				Locked = Mutex.TryLock();
				if (Locked && IsValid(this) && GEngine)
				{
					ACSGBaseCharacter* CSGCharacter = Cast<ACSGBaseCharacter>(GetOwner());
					if (CSGCharacter)
					{
						NewIlluminationLevel = CSGCharacter->GetFullLightIllumination();
						IlluminationLevel = NewIlluminationLevel;
					}
					GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, FString::Printf(TEXT("IlluminationLevel: %s"), *FString::SanitizeFloat(NewIlluminationLevel)));
					Mutex.Unlock();
				}
				FPlatformProcess::Sleep(0.01f);
				/*
				AsyncTask(ENamedThreads::GameThread, [this, NewIlluminationLevel]()
				{
					if (IsValid(this))
					{
						this->IlluminationLevel = NewIlluminationLevel;
						GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, FString::Printf(TEXT("IlluminationLevel: %s"), *FString::SanitizeFloat(NewIlluminationLevel)));
					}
				});*/
			}
	});
}

float UIlluminationAnalyzerComponent::GetIlluminationLevel()
{
	float CurrentIlluminationLevel{};
	bool Locked = Mutex.TryLock();
	if (Locked && IsValid(this))
	{
		CurrentIlluminationLevel = IlluminationLevel;
	}
	return CurrentIlluminationLevel;
}
