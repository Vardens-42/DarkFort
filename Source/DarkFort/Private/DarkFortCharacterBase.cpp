// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkFortCharacterBase.h"

#include "DarkFortCharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/DarkFortAttributeSetBase.h"
#include "Character/Abilities/CharacterGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/DarkFortPlayerController.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADarkFortCharacterBase

ADarkFortCharacterBase::ADarkFortCharacterBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UDarkFortCharacterMovementComponent>(CharacterMovementComponentName))
{

	DarkFortCharacterMovementComponent = Cast<UDarkFortCharacterMovementComponent>(GetCharacterMovement());
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 422.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 10000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/*
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 4.f;
	CameraBoom->CameraLagMaxDistance = 3.f;

	/*Yucky rotation lag
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 4.f;
	CameraBoom->CameraLagMaxTimeStep = 1.f;*/

	/*
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	*/

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	bAlwaysRelevant = true;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

void ADarkFortCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (ADarkFortPlayerController* PlayerController = Cast<ADarkFortPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}	
}

UAbilitySystemComponent* ADarkFortCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool ADarkFortCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 ADarkFortCharacterBase::GetAbilityLevel(DarkFortAbilityID AbilityID) const
{
	return 1;
}

void ADarkFortCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven) 
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities()) {
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++) {
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void ADarkFortCharacterBase::Die()
{
	RemoveCharacterAbilities();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void ADarkFortCharacterBase::FinishDying()
{
	Destroy();
}

int32 ADarkFortCharacterBase::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid()) 
	{
		return AttributeSetBase->GetCharacterLevel();
	}
	return 0;
}

float ADarkFortCharacterBase::GetHealth() const
{
	if (AttributeSetBase.IsValid()) 
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float ADarkFortCharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid()) 
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ADarkFortCharacterBase::GetStamina() const
{
	if (AttributeSetBase.IsValid()) 
	{
		return AttributeSetBase->GetStamina();
	}
	return 0.0f;
}

float ADarkFortCharacterBase::GetMaxStamina() const
{
	if (AttributeSetBase.IsValid()) 
	{
		return AttributeSetBase->GetMaxStamina();
	}
	return 0.0f;
}

void ADarkFortCharacterBase::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven) 
	{
		return;
	}

	for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility:CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void ADarkFortCharacterBase::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ADarkFortCharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->StartupEffectsApplied) 
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void ADarkFortCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void ADarkFortCharacterBase::SetStamina(float Stamina)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADarkFortCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADarkFortCharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADarkFortCharacterBase::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ADarkFortCharacterBase::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADarkFortCharacterBase::StopSprinting);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADarkFortCharacterBase::ToggleCrouch);

		// Walking
		//EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ADarkFortCharacterBase::ToggleWalk);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADarkFortCharacterBase::Move(const FInputActionValue& Value)
{
	if (Controller && IsAlive())
	{
		// input is a Vector2D
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// find out which way is forward
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (MovementVector.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MovementVector.X);
		}

		if (MovementVector.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MovementVector.Y);
		}
	}
		/*
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		*/
}

void ADarkFortCharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// add yaw and pitch input to controller
		if (LookAxisVector.X != 0.0f)
		{
			AddControllerYawInput(LookAxisVector.X);
		}
		if (LookAxisVector.Y != 0.0f)
		{
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

FCollisionQueryParams ADarkFortCharacterBase::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ADarkFortCharacterBase::Jump()
{
	Super::Jump();
	bPressedDarkFortJump = true;
	//bPressedJump = false;
}

void ADarkFortCharacterBase::StopJumping()
{
	Super::StopJumping();
	bPressedDarkFortJump = false;
}

void ADarkFortCharacterBase::StartSprint()
{
	DarkFortCharacterMovementComponent->SprintPressed();
}

void ADarkFortCharacterBase::StopSprinting()
{
	DarkFortCharacterMovementComponent->SprintReleased();
}

void ADarkFortCharacterBase::ToggleCrouch()
{
	DarkFortCharacterMovementComponent->CrouchPressed();
}