// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MeatPlayerController.h"
#include "Interactable.h"

#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.f);

	// Set turn rates
	BaseTurnRate = 45.f;
	BaseLookupRate = 45.f;

	// Block character rotation when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FearAmount = 1.f;
	MaxFearAmount = 1000.f;
	StartReducingFear = false;
}

void APlayerCharacter::SetLeftHandEquipped(bool Value)
{
	bLeftHandEquipped = Value;
}

void APlayerCharacter::SetRightHandEquipped(bool Value)
{
	bRightHandEquipped = Value;
}

bool APlayerCharacter::GetTwoHandedEquip() const
{
	return bLeftHandEquipped && bRightHandEquipped;
}

void APlayerCharacter::SetTwoHandedEquip(bool Value)
{
	bLeftHandEquipped = Value;
	bRightHandEquipped = Value;
}

void APlayerCharacter::CalculateFear(float ExponentialFactor)
{
	FearAmount += ExponentialFactor;
	if (FearAmount > MaxFearAmount)
	{
		FearAmount = MaxFearAmount;
		UE_LOG(LogTemp, Warning, TEXT("You are freaked the fuck out!"));
	}
	if (FearAmount < 1.f)
	{
		FearAmount = 1.f;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForInteractables();
	if (StartReducingFear)
	{
		FearAmount -= 1.f;
		if (FearAmount <= 1.f)
		{
			StartReducingFear = false;
			UE_LOG(LogTemp, Warning, TEXT("Whew! That was close! You're feeling a little better now."));
		}
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);
}

void APlayerCharacter::MoveForward(float Value)
{
	YawRotateOnAxis(Value, EAxis::X);
}

void APlayerCharacter::MoveRight(float Value)
{
	YawRotateOnAxis(Value, EAxis::Y);
}

void APlayerCharacter::YawRotateOnAxis(float Value, EAxis::Type type)
{
	if ((Controller != nullptr) && Value != 0.f)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(type);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::CheckForInteractables()
{
	// Create a LineTrace to check for a hit
	FHitResult HitResult;

	int32 Range = 700;
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * Range) + StartTrace;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Emerald, false, .0167, '\000', 1.f);

	AMeatPlayerController* PlayerController = Cast<AMeatPlayerController>(GetController());
	if (PlayerController)
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, Params))
		{
			AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());
			if (Interactable)
			{
				PlayerController->CurrentInteractable = Interactable;
				return;
			}
		}

		PlayerController->CurrentInteractable = nullptr;
	}
}
