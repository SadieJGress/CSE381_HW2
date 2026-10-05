// Fill out your copyright notice in the Description page of Project Settings.
#include "RoofHatch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaGameMode.h"
#include "LavaHuangGressCharacter.h"
#include "Kismet/GameplayStatics.h"

ARoofHatch::ARoofHatch()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene Root
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Hatch mesh
	HatchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatchMesh"));
	HatchMesh->SetupAttachment(RootComponent);
	HatchMesh->SetCollisionProfileName(TEXT("BlockAll"));

	// Victory hitbox tigger
	VictoryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryTrigger"));
	VictoryTrigger->SetupAttachment(RootComponent);
	VictoryTrigger->SetCollisionProfileName(TEXT("Trigger"));
	VictoryTrigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
}

void ARoofHatch::BeginPlay()
{
	Super::BeginPlay();

	if (VictoryTrigger)
	{
		VictoryTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARoofHatch::HandleTriggerOverlap);
	}
}

void ARoofHatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Poll GameMode: unlock once player collects required keys (3)
	if (!IsOpen)
	{
		if (ALavaGameMode* GM = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (GM->GetKeysCollected() >= GM->GetKeysRequired())
			{
				UnlockHatch();
			}
		}
	}
}

void ARoofHatch::UnlockHatch()
{
	IsOpen = true;

	if (HatchMesh)
	{
		// Destroy hatch mesh
		HatchMesh->DestroyComponent();
		HatchMesh = nullptr;
	}

}

void ARoofHatch::HandleTriggerOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& Sweep)
{
	if (!IsOpen) return;

	// Victory triggered
	if (Cast<ALavaHuangGressCharacter>(OtherActor))
	{
		if (ALavaGameMode* GM = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->ReportHatchReached();
		}
	}
}