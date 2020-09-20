// Fill out your copyright notice in the Description page of Project Settings.


#include "MeatPlayerController.h"
#include "Interactable.h"
#include "Pickup.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMeshSocket.h"

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
	InputComponent->BindAction("RMB", IE_Pressed, this, &AMeatPlayerController::RMBDown);
	InputComponent->BindAction("RMB", IE_Released, this, &AMeatPlayerController::RMBUp);

	InputComponent->BindAction("LeftUnequip", IE_Pressed, this, &AMeatPlayerController::LeftUnequip);
	InputComponent->BindAction("RightUnequip", IE_Pressed, this, &AMeatPlayerController::RightUnequip);
}

void AMeatPlayerController::DisplayAimReticle_Implementation()
{
}

void AMeatPlayerController::RemoveAimReticle_Implementation()
{
}

void AMeatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APlayerCharacter>(GetCharacter());
}

void AMeatPlayerController::LeftUnequip()
{
	if (PC && PC->GetLeftHandEquipped())
	{
		TArray<AActor*>AttachedActors;
		PC->GetAttachedActors(AttachedActors);
		for (AActor* Item : AttachedActors)
		{
			APickup* Pickup = Cast<APickup>(Item);
			if (Pickup && (Pickup->GetCarryType() == ECarryType::ECT_LeftHand || Pickup->GetCarryType() == ECarryType::ECT_TwoHanded))
			{
				Item->Destroy();
				PC->SetLeftHandEquipped(false);
			}
		}
	}
}

void AMeatPlayerController::RightUnequip()
{
	if (PC && PC->GetRightHandEquipped())
	{
		TArray<AActor*>AttachedActors;
		PC->GetAttachedActors(AttachedActors);
		for (AActor* Item : AttachedActors)
		{
			APickup* Pickup = Cast<APickup>(Item);
			if (Pickup && (Pickup->GetCarryType() == ECarryType::ECT_RightHand))
			{
				Item->Destroy();
				PC->SetRightHandEquipped(false);
			}
		}
	}
}

void AMeatPlayerController::RMBDown()
{
	bRMBDown = true;
	if (AimReticleAsset)
	{
		AimReticleOverlay = CreateWidget<UUserWidget>(this, AimReticleAsset);
	}
	AimReticleOverlay->AddToViewport();
	AimReticleOverlay->SetVisibility(ESlateVisibility::Visible);

	// Here you may want to add a check to make sure no menus are open as well. Such as inventory or game settings.
	if (PC)
	{
		PC->bUseControllerRotationYaw = true;
	}
}

void AMeatPlayerController::RMBUp()
{
	bRMBDown = false;
	AimReticleOverlay->RemoveFromParent();
	if (PC)
	{
		PC->bUseControllerRotationYaw = false;
	}
}
