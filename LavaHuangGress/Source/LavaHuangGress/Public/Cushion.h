#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Cushion.generated.h"

class UBoxComponent;

UCLASS()
class LAVAHUANGGRESS_API ACushion : public AThrowable
{
    GENERATED_BODY()

public:
    ACushion();

    virtual void Throw(const FVector& Direction) override;

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
        const FHitResult& Hit) override;

    void Land(float SurfaceZ);

    void CheckLava();

    UFUNCTION(BlueprintImplementableEvent, Category = "Cushion")
    void OnLanded();

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float BurnSeconds = 5.f;

    UPROPERTY(EditAnywhere, Category = "Tuning")
    float LandedZOffset = 5.f;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "1.0"))
    float LandedScaleMultiplier = 3.f;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float GrowSeconds = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float ShrinkSeconds = 0.3f;

    bool bLanded = false;
    bool bFalling = false;
    bool bGrowing = false;
    bool bShrinking = false;
    float GrowElapsed = 0.f;
    float LandedElapsed = 0.f;

    FVector StartScale = FVector::OneVector;
    FVector TargetScale = FVector::OneVector;
    FVector ShrinkStartScale = FVector::OneVector;

    FTimerHandle LavaCheckTimer;

    TWeakObjectPtr<UBoxComponent> LavaVolume;

    
};