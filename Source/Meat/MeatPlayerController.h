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
	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> AimReticleAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* AimReticleOverlay;

	void Interact();

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widgets")
	void DisplayAimReticle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widgets")
	void RemoveAimReticle();

protected:
	void BeginPlay() override;

private:
	class APlayerCharacter* PC;
	bool bRMBDown;
	bool bResetAimReticle;

	void LeftUnequip();
	void RightUnequip();
	void RMBDown();
	void RMBUp();
};
