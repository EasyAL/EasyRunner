// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EasyRunnerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

//	defining event dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);

UCLASS(config=Game)
class AEasyRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseGameAction;

	/** Slide Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;

public:
	AEasyRunnerCharacter();
	

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	
	//	Called when pausing game
	void PauseGame(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//	Creating event dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FTestDelegate OnTestDelegate;

	//UFUNCTION()
	//void TestFunction();

protected:
	UPROPERTY(BlueprintReadWrite)
	float maxDeceleration;

	UPROPERTY(BlueprintReadWrite)
	float slowDownStamina;

	UPROPERTY(BlueprintReadWrite)
	float MaxSlowDownStamina;

	UPROPERTY(BlueprintReadWrite)
	float speedUpStamina;

	UPROPERTY(BlueprintReadWrite)
	float MaxSpeedUpStamina;

	UPROPERTY(BlueprintReadWrite)
	float maxAceleration;

	UPROPERTY(BlueprintReadWrite)
	bool canSpeedUp;

	UPROPERTY(BlueprintReadWrite)
	bool canSlowDown;
};

