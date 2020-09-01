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

	FString Carry = SwitchOnCarryType();
	FString Equippable = bCanBeEquipped ? "true" : "false";

	UE_LOG(LogTemp, Warning, TEXT("Can be Equipped: %s\nCarry Type: %s"), *Equippable, *Carry)
}

FString APickup::SwitchOnCarryType()
{
	FString Type;
	switch (CarryType)
	{
	case ECarryType::ECT_ERROR:
		Type = "Error";
		break;
	case ECarryType::ECT_NoEquip:
		Type = "Non equipable object";
		break;
	case ECarryType::ECT_LeftHand:
		Type = "Left handed object";
		break;
	case ECarryType::ECT_RightHand:
		Type = "Right Handed object";
		break;
	case ECarryType::ECT_TwoHanded:
		Type = "Two handed object";
		break;
	}
	return Type;
}
