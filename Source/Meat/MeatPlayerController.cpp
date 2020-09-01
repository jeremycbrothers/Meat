// Fill out your copyright notice in the Description page of Project Settings.


#include "MeatPlayerController.h"
#include "Interactable.h"
#include "Pickup.h"
#include "PlayerCharacter.h"
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
	InputComponent->BindAction("LeftUnequip", IE_Pressed, this, &AMeatPlayerController::LeftUnequip);
	InputComponent->BindAction("RightUnequip", IE_Pressed, this, &AMeatPlayerController::RightUnequip);
}

void AMeatPlayerController::LeftUnequip()
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetCharacter());
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
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetCharacter());
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
