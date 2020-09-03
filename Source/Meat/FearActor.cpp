// Fill out your copyright notice in the Description page of Project Settings.


#include "FearActor.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFearActor::AFearActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
	DetectionSphere->SetSphereRadius(500.f);
	FearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FearMesh"));
	FearMesh->SetupAttachment(GetRootComponent());
	SphereRadius = 500.f;
	bCanDrawVector = false;
	ExponentialFactor = 1.f;
}

// Called when the game starts or when spawned
void AFearActor::BeginPlay()
{
	Super::BeginPlay();
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFearActor::DetectionSphereOnOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AFearActor::DetectionSphereOnOverlapEnd);
}

void AFearActor::DetectionSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PlayerCharacterRef = Cast<APlayerCharacter>(OtherActor);
		bCanDrawVector = true;
	}
}

void AFearActor::DetectionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacterRef)
	{
		bCanDrawVector = false;
		ExponentialFactor = 1.f;
		PlayerCharacterRef->StartReducingFear = true;
		PlayerCharacterRef = nullptr;
	}
}

// Called every frame
void AFearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanDrawVector)
	{
		if (PlayerCharacterRef)
		{
			// Create a LineTrace to check for a hit
			FHitResult HitResult;

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			
			// Create the data for the line trace vector
			int32 Range = SphereRadius;
			FVector StartTrace = GetActorLocation() + FVector(0.f,0.f,90.f); 
			FVector PlayerEnd = PlayerCharacterRef->GetActorLocation() + FVector(0.f, 0.f, 90.f);
			FVector EndTrace = PlayerEnd; // This needs to be the end of the sphere or the location of the OtherActor. Which ever is closest.
			
			FVector SpaceBetweenPlayerAndFear = StartTrace - EndTrace; // Distance between fear object and player

			// Only increase or decrease the amount of fear to apply if current vector is not the same size as the previous vector.
			// Otherwise, we set the exponential factor is zero. That way the fear does not increase or decrease, because neither the player, nor 
			// the fear object is moving towards or away from each other.
			if (SpaceBetweenPlayerAndFear.Size() != PlayerCharacterRef->PreviousLocation.Size())
			{
				// If the current vector is larger than the previous vector, the player is fruther away from the fear object.
				// Otherwise, the player is closer and we'll want to increase the amount of the expontial factor.
				if (SpaceBetweenPlayerAndFear.Size() > PlayerCharacterRef->PreviousLocation.Size() && !FMath::IsNearlyZero(ExponentialFactor))
				{
					// We want to decrease the players fear as they move further from the fear object. So, if the exponential factor is positive, we it to zero.
					// Then, we start decreasing the exponential factor.
					if (ExponentialFactor > 0.f)
					{
						ExponentialFactor = 0.f;
					}
					ExponentialFactor -= 0.01f;
				}
				else
				{
					ExponentialFactor += 0.02f;
				}
			}
			else
			{
				if (ExponentialFactor <= 0.f)
				{
					ExponentialFactor = 0.f;
				}
			}

			PlayerCharacterRef->PreviousLocation = SpaceBetweenPlayerAndFear;

			UWorld* World = GetWorld();
			if (World)
			{
				World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, Params);
				APlayerCharacter* PC = Cast<APlayerCharacter>(HitResult.GetActor());
				if (PC)
				{
					//DrawDebugLine(World, StartTrace, EndTrace, FColor(255, 0, 0), false, 0.016, 0, 5.f);
					PlayerCharacterRef->CalculateFear(ExponentialFactor);
				}
			}
		}
	}
}

