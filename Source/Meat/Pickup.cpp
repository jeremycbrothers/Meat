// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

APickup::APickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	Super::Name = "Item";
	Super::Action = "pickup";

	bCanBeEquipped = false;
	CarryType = ECarryType::ECT_NoEquip;
}

void APickup::Interact_Implementation(APlayerController* Controller)
{
	Super::Interact_Implementation(Controller);
}