#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Extinguisher.generated.h"

UCLASS()
class LAVAHUANGGRESS_API AExtinguisher : public AThrowable
{
    GENERATED_BODY()

public:
    AExtinguisher();

protected:
    virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Extinguisher")
    void OnHitTarget();

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float MissLifeSeconds = 10.f;

    bool bSpent = false;
};