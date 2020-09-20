// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MEAT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FVector PreviousLocation;

	bool StartReducingFear;

	// Sets default values for this character's properties
	APlayerCharacter();

	bool GetLeftHandEquipped() const { return bLeftHandEquipped; }
	void SetLeftHandEquipped(bool Value);

	bool GetRightHandEquipped() const { return bRightHandEquipped; }
	void SetRightHandEquipped(bool Value);

	bool GetTwoHandedEquip() const;
	void SetTwoHandedEquip(bool Value);

	void CalculateFear(float ExponentialFactor);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom suboject. */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject. */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate, in degrees/seconds. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base look up/down rate in degrees/second. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookupRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	bool bLeftHandEquipped;

	bool bRightHandEquipped;
	
	float FearAmount;

	float MaxFearAmount;

	/** Called for forwards/backwards input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Handles both forward or right input */
	void YawRotateOnAxis(float Value, EAxis::Type type);

	/** Checks for interactable actors every fram (used in tick function */
	void CheckForInteractables();
};
