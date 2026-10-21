#include "Cushion.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Lava.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ACushion::ACushion()
{
    Mesh->SetGenerateOverlapEvents(false);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACushion::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bLanded) return;

    if (Cast<ALava>(OtherActor))
    {
        Land(Hit.ImpactPoint.Z);
        return;
    }

    if (bFalling) return;

    bFalling = true;
    Movement->StopMovementImmediately();
    Movement->Deactivate();
    Mesh->SetSimulatePhysics(true);
}

void ACushion::Land(float SurfaceZ)
{
    bLanded = true;

    Movement->StopMovementImmediately();
    Movement->Deactivate();
    Mesh->SetSimulatePhysics(false);

    FVector Loc = GetActorLocation();
    Loc.Z = SurfaceZ + LandedZOffset;
    SetActorLocation(Loc);
    SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));

    StartScale = GetActorScale3D();
    TargetScale = StartScale * LandedScaleMultiplier;
    GrowElapsed = 0.f;
    bGrowing = true;
    
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

    SetLifeSpan(BurnSeconds);
    OnLanded();
    SetActorTickEnabled(true);
}    

void ACushion::Throw(const FVector& Direction)
{
    Super::Throw(Direction);
    SetActorRotation(FRotator(0.f, GetActorLocation().Yaw, 0.f));
    GetWorldTimerManager().SetTimer(LavaCheckTimer, this, &ACushion::CheckLava, 0.02f, true);
}

void ACushion::EndPlay(const EEndPlayReason::Type Reason)
{
    GetWorldTimerManager().ClearTimer(LavaCheckTimer);
    Super::EndPlay(Reason);
}

void ACushion::CheckLava()
{
    if (bLanded)
    {
        GetWorldTimerManager().ClearTimer(LavaCheckTimer);
        return;
    }

    AActor* LavaActor = UGameplayStatics::GetActorOfClass(this, ALava::StaticClass());
    if (!IsValid(LavaActor)) return;

    UBoxComponent* VolumeBox = LavaActor->FindComponentByClass<UBoxComponent>();
    if (!VolumeBox) return;

    const FBox Box = VolumeBox->Bounds.GetBox();
    const FVector P = GetActorLocation();

    const bool bInsideXY = P.X >= Box.Min.X && P.X <= Box.Max.X && P.Y >= Box.Min.Y && P.Y <= Box.Max.Y;

    if (bInsideXY && P.Z <= Box.Max.Z)
    {
        LavaVolume = VolumeBox;
        GetWorldTimerManager().ClearTimer(LavaCheckTimer);
        Land(Box.Max.Z);
    }
}

void ACushion::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bLanded || !LavaVolume.IsValid()) return;

    LandedElapsed += DeltaTime;
    const float ShrinkStart = FMath::Max(0.f, BurnSeconds - ShrinkSeconds);

    if (!bShrinking && ShrinkSeconds > 0.f && LandedElapsed >= ShrinkStart)
    {
        bShrinking = true;
        bGrowing = false;
        ShrinkStartScale = GetActorScale3D();
    }

    if (bShrinking)
    {
        const float Alpha = FMath::Clamp((LandedElapsed - ShrinkStart) / ShrinkSeconds, 0.f, 1.f);
        const float Eased = FMath::InterpEaseIn(0.f, 1.f, Alpha, 2.f);
        SetActorScale3D(FMath::Lerp(ShrinkStartScale, ShrinkStartScale * 0.01f, Eased));
    }
    else if (bGrowing)
    {
        GrowElapsed += DeltaTime;
        const float Alpha = (GrowSeconds > 0.f) ? FMath::Clamp(GrowElapsed / GrowSeconds, 0.f, 1.f) : 1.f;
        const float Eased = FMath::InterpEaseOut(0.f, 1.f, Alpha, 2.f);
        SetActorScale3D(FMath::Lerp(StartScale, TargetScale, Eased));
        if (Alpha >= 1.f) bGrowing = false;
    }

    if (!LavaVolume.IsValid()) return;

    FVector Loc = GetActorLocation();
    Loc.Z = LavaVolume->Bounds.GetBox().Max.Z + LandedZOffset;
    SetActorLocation(Loc);
}