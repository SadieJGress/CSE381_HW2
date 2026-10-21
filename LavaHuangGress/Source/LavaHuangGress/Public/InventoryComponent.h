// InventoryComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AThrowable;

UENUM(BlueprintType)
enum class EItemSlot : uint8
{
    Cushion      UMETA(DisplayName = "Cushion"),
    Extinguisher UMETA(DisplayName = "Extinguisher")
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    // Which Blueprint to spawn when this slot is thrown.
    UPROPERTY(EditDefaultsOnly, Category = "Slot")
    TSubclassOf<AThrowable> ItemClass;

    UPROPERTY(EditDefaultsOnly, Category = "Slot", meta = (ClampMin = "1"))
    int32 Capacity = 3;

    UPROPERTY(BlueprintReadOnly, Category = "Slot")
    int32 Count = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAVAHUANGGRESS_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    /** Returns false when that slot is already full. */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(EItemSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void EquipSlot(EItemSlot Slot);

    /** Spawns the equipped item and throws it. Does nothing if the slot is empty. */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ThrowEquipped();

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetCount(EItemSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    EItemSlot GetEquippedSlot() const { return EquippedSlot; }

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefillAllSlots();

protected:
    /** Destroy whatever is in the hand and attach one of the newly equipped item. */
    void RefreshHeldActor();

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemSlot, FInventorySlot> Slots;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    EItemSlot EquippedSlot = EItemSlot::Cushion;

    // Transient: runtime only, never saved, but the GC still needs to see it.
    UPROPERTY(Transient)
    TObjectPtr<AThrowable> HeldActor;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    FName HandSocketName = TEXT("hand_r");
};
