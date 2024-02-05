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
	CMOVE_Prone UMETA(DisplayName = "Prone"),
	CMOVE_MAX	UMETA(Hidden),
};

UCLASS()
class DARKFORT_API UDfCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Df : public FSavedMove_Character
	{
	public:
		enum CompressedFlags
		{
			FLAG_Sprint = 0x10,
			FLAG_Custom_1 = 0x20,
			FLAG_Custom_2 = 0x40,
			FLAG_Custom_3 = 0x80,
		};

		// Flags
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bPressedDarkFortJump : 1;

		//Other Variables
		uint8 Saved_bPrevWantsToCrouch : 1;
		uint8 Saved_bWantsToProne : 1;

		FSavedMove_Df();

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
	UPROPERTY(EditDefaultsOnly) float MaxSprintSpeed = 630.f;
	UPROPERTY(EditDefaultsOnly) float MaxJogSpeed = 422.f;

	//Required speed to slide or maintain slide
	UPROPERTY(EditDefaultsOnly) float MinSlideSpeed = 400.f;
	//The top speed you can move while sliding - no supersliding when going downhill
	UPROPERTY(EditDefaultsOnly) float MaxSlideSpeed = 400.f;
	//Boost in velocity you get when you enter slide
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse = 500;
	//Gravity needed to keep the player along the ground and also affects how fast a slope will change your velocity
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce = 5000;
	//How fast you lose your velocity as you slide
	UPROPERTY(EditDefaultsOnly) float SlideFrictionFactor = .06f;
	//
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding = 1000.f;

	//How long to hold prone input
	UPROPERTY(EditDefaultsOnly) float Prone_EnterHoldDuration = 0.2f;
	//Boost in velocity when entering prone from a slide
	UPROPERTY(EditDefaultsOnly) float ProneSlideEnterImpulse = 300.f;
	//Max speed when proning
	UPROPERTY(EditDefaultsOnly) float MaxProneSpeed = 300.f;
	//
	UPROPERTY(EditDefaultsOnly) float BreakingDecelerationProning = 2500.f;

	//Transient
	UPROPERTY(Transient) ADarkFortCharacter* DarkFortCharacterOwner;
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;
	FTimerHandle TimerHandle_EnterProne;

public:
	UDfCharacterMovementComponent();

	// Actor Component
protected:
	virtual void InitializeComponent();

	//Character Movement Component
public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
public:	
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	//Slide
private:
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	bool CanSlide() const;
	void PhysSlide(float deltaTime, int32 Iterations);

	//Prone
private:
	void TryEnterProne() {Safe_bWantsToProne = true;}
	UFUNCTION(Server, Reliable) void Server_EnterProne();

	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iterations);

	//Interface
public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Sprinting") bool bIsSprinting;

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
