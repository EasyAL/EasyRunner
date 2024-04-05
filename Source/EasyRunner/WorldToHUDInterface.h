// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorldToHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI /*, Blueprintable*/)
class UWorldToHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EASYRUNNER_API IWorldToHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "World to HUD Interface")
	void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "World to HUD Interface")
	void DisplayGameOverScreen();
};
