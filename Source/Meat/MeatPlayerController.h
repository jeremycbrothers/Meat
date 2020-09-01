// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MEAT_API AMeatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AInteractable* CurrentInteractable;

protected:
	void Interact();

	virtual void SetupInputComponent() override;
};
