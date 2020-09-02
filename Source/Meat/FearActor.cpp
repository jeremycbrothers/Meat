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
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	if (OtherActor)
	{
		PlayerCharacterRef = Cast<APlayerCharacter>(OtherActor);
		bCanDrawVector = true;
		bCanStartDecreaseFear = false;
	}
}

void AFearActor::DetectionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacterRef)
	{
		PlayerCharacterRef = nullptr;
		bCanDrawVector = false;
		bCanStartDecreaseFear = true;
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

			int32 Range = SphereRadius;
			FVector StartTrace = GetActorLocation(); // Really this needs to be arround the top of the mesh.
			FVector EndTrace(0.f, 0.f, 0.f);
			EndTrace = PlayerCharacterRef->GetActorLocation(); // This needs to be the end of the sphere or the location of the OtherActor. Which ever is closest.
			if(PlayerCharacterRef->PreviousLocation == FVector(0.f,0.f,0.f))
				PlayerCharacterRef->PreviousLocation = EndTrace;
			if (PlayerCharacterRef->PreviousLocation != EndTrace)
			{
				float Length = PlayerCharacterRef->PreviousLocation.Size() - EndTrace.Size();
				if (Length > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("closer"));
					PlayerCharacterRef->PreviousLocation = EndTrace;
					ExponentialFactor *= 1.01f;
				}
				else if (Length < 0 && ExponentialFactor > 0.9f)
				{
					UE_LOG(LogTemp, Warning, TEXT("away"));
					ExponentialFactor /= 1.01f;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("equal"));
			}
			
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			UWorld* World = GetWorld();
			if (World)
			{
				World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, Params);
				DrawDebugLine(World, StartTrace, EndTrace, FColor(255, 0, 0), false, 0.016, 0, 5.f);
				APlayerCharacter* PC = Cast<APlayerCharacter>(HitResult.GetActor());
				if (PC)
				{
					PlayerCharacterRef->CalculateFear(ExponentialFactor);
				}
			}
		}
	}

	if (bCanStartDecreaseFear && ExponentialFactor >= 0.9f)
	{
		ExponentialFactor /= 1.01f;
		//UE_LOG(LogTemp, Warning, TEXT("Factor: %f"), ExponentialFactor);
	}
}

