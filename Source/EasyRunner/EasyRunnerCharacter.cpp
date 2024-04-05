// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyRunnerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEasyRunnerCharacter

AEasyRunnerCharacter::AEasyRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	maxDeceleration = -0.74f;

	maxAceleration = 0.25f;

	canSlowDown = true;
	canSpeedUp = true;

	MaxSpeedUpStamina = 100.0f;
	MaxSlowDownStamina = 100.0f;

	speedUpStamina = MaxSpeedUpStamina;
	slowDownStamina = MaxSlowDownStamina;

	//OnTestDelegate.AddDynamic(this, &AMyPlayerController::TestFunction);

}

void AEasyRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEasyRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEasyRunnerCharacter::Move);

		// Pausing Game
		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &AEasyRunnerCharacter::PauseGame);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEasyRunnerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	float MovementVector = Value.Get<float>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (MovementVector > 0 && canSpeedUp)
		{
			// add forward movement 
			AddMovementInput(ForwardDirection, maxAceleration);
			speedUpStamina -= 0.1f;

			if (speedUpStamina < 0)
			{
				canSpeedUp = false;
				OnTestDelegate.Broadcast();
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current Speed Up Stamina: %f"), speedUpStamina));
		}
		else if(MovementVector < 0 && canSlowDown)
		{
			// add backward movement 
			AddMovementInput(ForwardDirection, maxDeceleration);
			slowDownStamina -= 0.1f;

			if (slowDownStamina < 0)
			{
				canSlowDown = false;
				OnTestDelegate.Broadcast();
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current Slow Down Stamina: %f"), slowDownStamina));
		}
		//UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), MovementVector.X);
	}
}

void AEasyRunnerCharacter::PauseGame(const FInputActionValue& Value)
{
	//AHUD* PlayerHUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();

	/*
	if (PlayerHUD->GetClass()->ImplementsInterface(IInterface_MyInterface::StaticClass()))
	{
		IInterface_MyInterface::Execute_MyInterfaceFunction(MyActor);
	}*/
	/*
	if (PlayerHUD->GetClass()->ImplementsInterface(IInterface_MyInterface::StaticClass()))
	{
		IInterface_BPI_UIManagerCommunication::Execute_PauseTheGame(PlayerHUD);
	}*/

	//IBPI_UIManagerCommunication* s = Cast<IBPI_UIManagerCommunication>(PlayerHUD);
}
