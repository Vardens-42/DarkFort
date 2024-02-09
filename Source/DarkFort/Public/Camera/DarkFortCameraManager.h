#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DarkFortCameraManager.generated.h"

UCLASS()
class DARKFORT_API ADarkFortCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration = .25f;
	float CrouchBlendTime;

public:
	// Sets default values for this empty's properties
	ADarkFortCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void UpdateCameraLocation(float DeltaTime, FMinimalViewInfo& OutCameraView);
};
