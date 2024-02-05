#include "DarkFortCameraManager.h"
#include "DarkFortCharacter.h"
#include "DarkFortPlayerController.h"
#include "DarkFortCharacterMovementComponent.h"
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
    // TODO: track multiple players, predict movement direction, maybe track enemies, etc.
	ADarkFortPlayerController* DarkFortPlayerController = Cast<ADarkFortPlayerController>(GetOwningPlayerController());
    if (ADarkFortCharacter* DarkFortCharacter = Cast<ADarkFortCharacter>(DarkFortPlayerController->GetPawn()))
	{	
        // CameraRotation and CameraDistance are two variables that can be edited to adjust the camera viewpoint
        OutCameraView.Location = DarkFortCharacter->GetActorLocation() + (GetOwningPlayerController()->GetControlRotation().Vector() * -210);
        OutCameraView.Rotation = GetOwningPlayerController()->GetControlRotation();
    }
    else
    {
		// Fall back to default player controller view
		// TODO: potentially incorrect, maybe focusing the player start the player will spawn at while we
		// wait for him to spawn would be more correct
		DarkFortCharacter->GetActorEyesViewPoint(OutCameraView.Location, OutCameraView.Rotation);
    }
}