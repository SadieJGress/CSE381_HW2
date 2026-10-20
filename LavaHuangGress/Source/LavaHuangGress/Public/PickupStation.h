#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryComponent.h"
#include "PickupStation.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class LAVAHUANGGRESS_API APickupStation : public AActor
{
    GENERATED_BODY()

public:
    APickupStation();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                       bool bFromSweep, const FHitResult& Sweep);

    bool TryGive(AActor* Other);
    
    void Restock();
    void SetAvailable(bool bNewAvailable);

    UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
    void OnAvailabilityChanged(bool bNowAvailable);
    
    UPROPERTY(EditAnywhere, Category = "Pickup")
    EItemSlot ItemType = EItemSlot::Cushion;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float RestockSeconds = 15.f;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StockMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger;

    bool bAvailable = true;
    FTimerHandle RestockTimer;
};