// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FearActor.generated.h"

UCLASS()
class MEAT_API AFearActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFearActor();

	UFUNCTION()
	virtual void DetectionSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void DetectionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FearActor", meta = (AllowPrivateAccess="true"))
	class UStaticMeshComponent* FearMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FearActor", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionSphere;
	
	class APlayerCharacter* PlayerCharacterRef;
	float ExponentialFactor;
	float SphereRadius;
	bool bCanDrawVector;
};
