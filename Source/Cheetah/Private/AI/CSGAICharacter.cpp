// Copyright Threedking. All Rights Reserved.


#include "AI/CSGAICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

ACSGAICharacter::ACSGAICharacter()
{
    AutoPossessAI = EAutoPossessAI::Disabled;

    if (GetCharacterMovement())
    {
        bUseControllerRotationYaw = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}
