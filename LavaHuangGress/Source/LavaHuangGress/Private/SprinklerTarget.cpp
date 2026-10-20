#include "SprinklerTarget.h"
#include "Components/StaticMeshComponent.h"
#include "LavaKey.h"

ASprinklerTarget::ASprinklerTarget()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    Mesh->SetNotifyRigidBodyCollision(true);
}

void ASprinklerTarget::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(LinkedKey))
    {
        LinkedKey->SetActorHiddenInGame(true);
        LinkedKey->SetActorEnableCollision(false);
    }
}

void ASprinklerTarget::OnHitByThrowable()
{
    if (bHit) return;
    bHit = true;

    if (HitMaterial)
    {
        Mesh->SetMaterial(0, HitMaterial);
    }

    if (IsValid(LinkedKey))
    {
        LinkedKey->SetActorHiddenInGame(false);
        LinkedKey->SetActorEnableCollision(true);
    }
    OnSprinklerOpened();
}