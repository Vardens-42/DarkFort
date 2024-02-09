#include "Camera/DarkFortCameraManager.h"
#include "Characters/DarkFortCharacter.h"
#include "Characters/DarkFortCharacterMovementComponent.h"
#include "Player/DarkFortPlayerController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ADarkFortCameraManager::ADarkFortCameraManager()
{

}

void ADarkFortCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ADarkFortCharacter* DarkFortCharacter = Cast<ADarkFortCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UDarkFortCharacterMovementComponent* DFMC = DarkFortCharacter->GetDarkFortCharacterMovement();
		FVector TargetCrouchOffset = FVector(0, 0, DFMC->GetCrouchedHalfHeight() - DarkFortCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (DFMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (DFMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}

void ADarkFortCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (BlueprintUpdateCamera(OutVT.Target, OutVT.POV.Location, OutVT.POV.Rotation, OutVT.POV.FOV))
    {
        return;
    }
 
    UpdateCameraLocation(DeltaTime, OutVT.POV);
}

void ADarkFortCameraManager::UpdateCameraLocation(const float DeltaTime, FMinimalViewInfo& OutCameraView)
{
    if (PCOwner != nullptr)
    {
        if (const APawn* Pawn = PCOwner->GetPawn())
        {
            OutCameraView.Location = Pawn->GetActorLocation() + (GetOwningPlayerController()->GetControlRotation().Vector() * -210);
            OutCameraView.Rotation = GetOwningPlayerController()->GetControlRotation();
        }
        else
        {
            PCOwner->GetActorEyesViewPoint(OutCameraView.Location, OutCameraView.Rotation);
        }
    }
}