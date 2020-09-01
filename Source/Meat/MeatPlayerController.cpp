// Fill out your copyright notice in the Description page of Project Settings.


#include "MeatPlayerController.h"
#include "Interactable.h"

void AMeatPlayerController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}

void AMeatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &AMeatPlayerController::Interact);
}
