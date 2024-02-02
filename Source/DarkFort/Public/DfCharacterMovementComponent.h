#pragma once

#include "CoreMinimal.h"
#include "DarkFortCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DfCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	UMETA(Hidden),
	CMOVE_Slide	UMETA(DisplayName = "Slide"),
	CMOVE_MAX	UMETA(Hidden),
};

UCLASS()
class DARKFORT_API UDfCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Df : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		// Flag
		uint8 Saved_bWantsToSprint : 1;

		uint8 Saved_bPrevWantsToCrouch : 1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Df : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Df(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	//Parameters
	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Jog_MaxWalkSpeed;

	//Required speed to slide or maintain slide
	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350;

	//Boost in velocity you get when you enter slide
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500;

	//Gravity needed to keep the player along the ground and also affects how fast a slope will change your velocity
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;

	//How fast you lose your velocity as you slide
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3;

	//Transient
	UPROPERTY(Transient) ADarkFortCharacter* DarkFortCharacterOwner;

	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;

public:
	UDfCharacterMovementComponent();

protected:
	virtual void InitializeComponent();

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

public:
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	//UFUNCTION(BlueprintCallable) void SetCMCValues();
};
