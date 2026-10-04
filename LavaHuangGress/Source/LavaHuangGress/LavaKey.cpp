#include "LavaKey.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaGameMode.h"
#include "GameFramework/Character.h"

ALavaKey::ALavaKey()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	RootComponent = PickupRange;
	PickupRange->setMobility(EComponentMobility::Movable);
	PickupRange->InitSphereRadius(100.f);
	PickupRange->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	AddActorLocalRotation(FRotator(0.f, SpinRate * DeltaTime, 0.f));
	const float Offset = FMath::Sin(GetWorld()->GetTimeSeconds() * BobSpeed) * BobAmplitude;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, Offset));
}

void ALavaKey::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	if (!Cast<ACharacter>(OtherActor)) 
	{
		return;
	}
	ALavaGameMode* GM = GetWorld()->GetAuthGameMode<ALavaGameMode>();
	if (GM) 
	{
		GM->ReportKeyCollected();
		Destroy();
	}
}
