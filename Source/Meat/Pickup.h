// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

UENUM(BlueprintType)
enum class ECarryType : uint8
{
	ECT_NoEquip,
	ECT_LeftHand,
	ECT_RightHand,
	ECT_TwoHanded,
	ECT_ERROR
};

/**
 * 
 */
UCLASS()
class MEAT_API APickup : public AInteractable
{
	GENERATED_BODY()
public:
	APickup();

	void Interact_Implementation(APlayerController* Controller) override;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	bool bCanBeEquipped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	ECarryType CarryType;

	FString CType;

	// Debug function
	FString SwitchOnCarryType();
};
