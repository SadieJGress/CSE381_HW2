// Lava.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lava.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class LAVAHUANGGRESS_API ALava : public AActor
{
    GENERATED_BODY()

public:
    ALava();

    virtual void Tick(float DeltaTime) override;

    /** How far the surface has risen since the game started. Drives the HUD. */
    UFUNCTION(BlueprintPure, Category = "Lava")
    float GetRiseHeight() const;

    void ToggleDebugSpeed();

    UFUNCTION(BlueprintPure, Category = "Lava")
    float GetRiseRate() const { return RiseRate * (bDebugFast ? DebugSpeedMultiplier : 1.f); }

    UFUNCTION(BlueprintPure, Category = "Lava")
    float GetBuildingHeight() const { return BuildingHeight; }

    // Drop lava amount when touched
    UFUNCTION(BlueprintCallable, Category = "Lava")
    void DropLavaLevel(float Amount);


protected:
    virtual void BeginPlay() override;

    // The signature must match FComponentBeginOverlapSignature exactly,
    // and it must be marked UFUNCTION or it will silently never be called.
    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& Sweep);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Surface;

    /** Sits just under the surface. This is what actually detects the player. */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> Volume;

    UPROPERTY(EditAnywhere, Category = "Debug", meta = (ClampMin = "1.0"))
    float DebugSpeedMultiplier = 10.f;

    bool bDebugFast = false;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "cm/s"))
    float RiseRate = 40.f;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "1.0", Units = "cm"))
    float BuildingHeight = 4900.f;

    /** Recorded at BeginPlay so GetRiseHeight has something to measure from. */
    float StartZ = 0.f;

    UPROPERTY(EditAnywhere, Category = "Lava")
    float LavaDropAmount = 200.0f;
};

