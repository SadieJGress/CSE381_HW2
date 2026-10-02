#include "Lava.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "LavaHuangGressCharacter.h"

ALava::ALava()
{
	PrimaryActorTick.bCanEverTick = true;

	Surface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Surface"));
	RootComponent = Surface;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Volume->SetupAttachment(RootComponent);
	Volume->SetCollisionProfileName(TEXT("Trigger"));
}

void ALava::BeginPlay()
{
	Super::BeginPlay();

	StartZ = GetActorLocation().Z;

	if (Volume)
	{
		Volume->OnComponentBeginOverlap.AddDynamic(this, &ALava::HandleOverlap);
	}
}

void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rise upward over time
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += RiseRate * DeltaTime;
	SetActorLocation(NewLocation);
}

float ALava::GetRiseHeight() const
{
	return GetActorLocation().Z - StartZ;
}

void ALava::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	// Check if the overlapping actor is the player character
	if (ALavaHuangGressCharacter* Player = Cast<ALavaHuangGressCharacter>(OtherActor))
	{
		if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			// Tell GameMode the player touched lava
			GameMode->ReportLifeLost();
		}
	}
}

