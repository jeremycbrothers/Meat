// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "PlayerCharacter.h"
#include "MeatPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"

APickup::APickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	Super::Name = "Item";
	Super::Action = "pickup";

	CarryType = ECarryType::ECT_NoEquip;
}

void APickup::Interact_Implementation(AMeatPlayerController* Controller)
{
	Super::Interact_Implementation(Controller);
	
	APlayerCharacter* PC = Cast<APlayerCharacter>(Controller->GetCharacter());
	if (PC)
	{
		// If the left hand is empty and picking up a left handed item.
		if (!PC->GetLeftHandEquipped() && CarryType == ECarryType::ECT_LeftHand)
		{
			const USkeletalMeshSocket* SocketToAttach = PC->GetMesh()->GetSocketByName("LeftHandSocket");
			if (SocketToAttach)
			{
				PickupMesh->SetSimulatePhysics(false);
				PickupMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				if (SocketToAttach->AttachActor(this, PC->GetMesh()))
				{
					PC->SetLeftHandEquipped(true);
				}
			}
		}

		// If the right hand is empty and picking up a right handed item.
		if (!PC->GetRightHandEquipped() && CarryType == ECarryType::ECT_RightHand)
		{
			const USkeletalMeshSocket* SocketToAttach = PC->GetMesh()->GetSocketByName("RightHandSocket");
			if (SocketToAttach)
			{
				PickupMesh->SetSimulatePhysics(false);
				PickupMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				if (SocketToAttach->AttachActor(this, PC->GetMesh()))
				{
					PC->SetRightHandEquipped(true);
				}
			}
		}

		// If the left and right hands are both empty and picking up a two handed item.
		if (!PC->GetLeftHandEquipped() && !PC->GetRightHandEquipped() && CarryType == ECarryType::ECT_TwoHanded)
		{
			const USkeletalMeshSocket* SocketToAttach = PC->GetMesh()->GetSocketByName("LeftHandSocket");
			if (SocketToAttach)
			{
				PickupMesh->SetSimulatePhysics(false);
				PickupMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				if (SocketToAttach->AttachActor(this, PC->GetMesh()))
				{
					PC->SetTwoHandedEquip(true);
				}
			}
		}
	}
}
