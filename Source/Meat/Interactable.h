// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class MEAT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	UFUNCTION(BlueprintNativeEvent)
		void Interact(APlayerController* Controller);
	virtual void Interact_Implementation(APlayerController* Controller);

	UPROPERTY(EditDefaultsOnly)
		FString Name;

	UPROPERTY(EditDefaultsOnly)
		FString Action;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		FString GetInteractText() const;

};
