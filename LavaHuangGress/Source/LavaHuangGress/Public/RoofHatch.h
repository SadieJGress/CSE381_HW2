// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoofHatch.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class LAVAHUANGGRESS_API ARoofHatch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoofHatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Victory Trigger handler  
	UFUNCTION()
	void HandleTriggerOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool FromSweep, const FHitResult& Sweep
	);

protected:
	// The hatch itself
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HatchMesh;

	// Victory trigger box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* VictoryTrigger;

	void UnlockHatch();

	// Check if hatch is open
	bool IsOpen = false;

};
