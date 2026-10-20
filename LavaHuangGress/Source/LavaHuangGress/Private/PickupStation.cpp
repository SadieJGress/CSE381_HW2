#include "PickupStation.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

APickupStation::APickupStation()
{
    PrimaryActorTick.bCanEverTick = false;
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    
    StockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StockMesh"));
    StockMesh->SetupAttachment(RootComponent);
    StockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetBoxExtent(FVector(120.f, 120.f, 120.f));
    Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Trigger->SetGenerateOverlapEvents(true);
}

void APickupStation::BeginPlay()
{
    Super::BeginPlay();

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickupStation::HandleOverlap);
}

void APickupStation::EndPlay(const EEndPlayReason::Type Reason)
{
    GetWorldTimerManager().ClearTimer(RestockTimer);
    Super::EndPlay(Reason);
}

void APickupStation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
    TryGive(OtherActor);
}

bool APickupStation::TryGive(AActor* Other)
{
    if (!bAvailable || !IsValid(Other) || !Cast<ACharacter>(Other)) return false;

    UInventoryComponent* Inv = Other->FindComponentByClass<UInventoryComponent>();
    if (!Inv) return false;

    if (!Inv->AddItem(ItemType)) return false;

    SetAvailable(false);
    GetWorldTimerManager().SetTimer(RestockTimer, this, &APickupStation::Restock, RestockSeconds, false);
    return true;
}

void APickupStation::Restock()
{
    SetAvailable(true);

    TArray<AActor*> Overlapping;
    Trigger->GetOverlappingActors(Overlapping, ACharacter::StaticClass());
    for (AActor* A : Overlapping)
    {
        if (TryGive(A)) break;
    }
}

void APickupStation::SetAvailable(bool bNewAvailable)
{
    bAvailable = bNewAvailable;
    StockMesh->SetVisibility(bAvailable, true);
    OnAvailabilityChanged(bAvailable);
}