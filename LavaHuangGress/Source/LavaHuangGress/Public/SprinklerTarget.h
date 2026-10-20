#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprinklerTarget.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class ALavaKey;

UCLASS()
class LAVAHUANGGRESS_API ASprinklerTarget : public AActor
{
    GENERATED_BODY()

public:
    ASprinklerTarget();
    
    void OnHitByThrowable();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditInstanceOnly, Category = "Target")
    TObjectPtr<ALavaKey> LinkedKey;

    UPROPERTY(EditAnywhere, Category = "Target")
    TObjectPtr<UMaterialInterface> HitMaterial;

    UFUNCTION(BlueprintImplementableEvent, Category = "Target")
    void OnSprinklerOpened();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    bool bHit = false;
};