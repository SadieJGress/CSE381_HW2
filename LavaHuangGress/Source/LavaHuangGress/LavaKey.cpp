#include "LavaKey.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ALavaKey::ALavaKey()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	PickupRange->SetupAttachment(RootComponent);
}

void ALavaKey::BeginPlay()
{
	Super::BeginPlay();

	if (PickupRange)
	{
		PickupRange->OnComponentBeginOverlap.AddDynamic(this, &ALavaKey::HandleOverlap);
	}
}

void ALavaKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALavaKey::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	// TODO: Report key pickup to GameMode
}
