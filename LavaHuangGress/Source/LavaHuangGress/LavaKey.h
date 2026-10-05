#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LavaKey.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class LAVAHUANGGRESS_API ALavaKey : public AActor
{
    GENERATED_BODY()

public:
    ALavaKey();

    /** Spin and bob, so the player can spot it from across the room. */
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& Sweep);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USphereComponent> PickupRange;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (Units = "deg/s"))
    float SpinRate = 90.f;

    UPROPERTY(EditAnywhere, Category = "Tuning")
    float BobSpeed = 2.f;

    UPROPERTY(EditAnywhere, Category = "Tuning")
    float BobAmplitude = 20.f;

    bool bCollected = false;
};

