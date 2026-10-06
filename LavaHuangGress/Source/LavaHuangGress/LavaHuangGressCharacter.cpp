// Copyright Epic Games, Inc. All Rights Reserved.

#include "LavaHuangGressCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "LavaHuangGress.h"
#include "DrawDebugHelpers.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

#define PRINT_LOG(Format, ...) \
    do { \
        FString _msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
        UE_LOG(LogTemp, Warning, TEXT("%s"), *_msg); \
        if (GEngine) { \
            GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, _msg); \
        } \
    } while(0)

ALavaHuangGressCharacter::ALavaHuangGressCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	// Increasing jump amount to two
	// Also gonna increase air control to .4
	// Okay had to do some voodoo and reset the ThirdPersonCharacter from being connected to ACharacter into being parented by this file
	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 625.f;
	GetCharacterMovement()->AirControl = 0.4f;
	GetCharacterMovement()->MaxWalkSpeed = 525.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ALavaHuangGressCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALavaHuangGressCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ALavaHuangGressCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALavaHuangGressCharacter::Look);
	}
	else
	{
		UE_LOG(LogLavaHuangGress, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALavaHuangGressCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ALavaHuangGressCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ALavaHuangGressCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ALavaHuangGressCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ALavaHuangGressCharacter::LavaHurt()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	// Freeze character
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetMesh()->bPauseAnims = true;


	// Reset after a second
	GetWorldTimerManager().SetTimer(
		HurtTimer,
		this,
		&ALavaHuangGressCharacter::ResetMovement,
		StunDuration,
		false
	);

	// Red camera effect
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraFade(
			0.8f, // Starting opacity
			0.0f, // End opacity
			0.5f, // Length
			FLinearColor::Red, // Color
			false,
			false
		);
	}
}

void ALavaHuangGressCharacter::ResetMovement()
{
	// Unpause character and respawn
	GetMesh()->bPauseAnims = false;

	RespawnAtSurface();

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}


void ALavaHuangGressCharacter::Jump()
{

	if (CanJump()) {
		// Check if character is in air 
		if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
		{
			// Play animation
			PlayAnimMontage(DoubleJumpMontage);

		}
	}

	// Call original Jump function
	Super::Jump();
}

void ALavaHuangGressCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ALavaHuangGressCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ALavaHuangGressCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Store spawn point as default safe respawn
	LastSafeLocation = GetActorLocation();
	LastSafeRotation = GetActorRotation();
}

void ALavaHuangGressCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Need to update surface now
	UpdateSafeSurface();
}

void ALavaHuangGressCharacter::UpdateSafeSurface()
{
	if (GetCharacterMovement() && GetCharacterMovement()->IsMovingOnGround())
	{
		FHitResult HitResult = GetCharacterMovement()->CurrentFloor.HitResult;

		if (HitResult.bBlockingHit && HitResult.GetActor() && !HitResult.GetActor()->IsA(ALava::StaticClass()))
		{
			ALava* LavaActor = Cast<ALava>(UGameplayStatics::GetActorOfClass(this, ALava::StaticClass()));
			float LavaZ = LavaActor ? LavaActor->GetActorLocation().Z : -99999.f;

			if (GetActorLocation().Z > LavaZ + 100.f)
			{
				LastSafeLocation = GetActorLocation();
				LastSafeRotation = GetActorRotation();

			}
		}
	}
}

void ALavaHuangGressCharacter::RespawnAtSurface()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

	// Debug
	PRINT_LOG("Respawning: %s", *LastSafeLocation.ToString());
	SetActorLocationAndRotation(LastSafeLocation, LastSafeRotation, false, nullptr, ETeleportType::ResetPhysics);
}

