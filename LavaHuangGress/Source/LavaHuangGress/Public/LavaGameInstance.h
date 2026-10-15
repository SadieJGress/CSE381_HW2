// LavaGameInstance.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LavaGameInstance.generated.h"

UCLASS()
class LAVAHUANGGRESS_API ULavaGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    /** Running score across both buildings. */
    UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 Score = 0;

    /** 0 for the first building, 1 for the second. */
    UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 BuildingIndex = 0;

    UFUNCTION(BlueprintCallable, Category = "Run")
    void AddScore(int32 Delta) { Score += Delta; }

    /** Call this when the player chooses Play Again. */
    UFUNCTION(BlueprintCallable, Category = "Run")
    void ResetRun();
};
