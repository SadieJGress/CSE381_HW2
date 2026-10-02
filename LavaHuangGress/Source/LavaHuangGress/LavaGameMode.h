// LavaGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LavaGameMode.generated.h"

class UResultWidget;

UCLASS()
class LAVAHUANGGRESS_API ALavaGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALavaGameMode();

    /** A key was picked up. The key itself does not know what that means. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportKeyCollected();

    /** The character touched lava. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportLifeLost();

    /** The player reached the hatch. The hatch does not check the keys itself. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportHatchReached();

    UFUNCTION(BlueprintPure, Category = "Rules")
    bool HasAllKeys() const { return KeysCollected >= KeysRequired; }

    UFUNCTION(BlueprintPure, Category = "Rules")
    float GetTimeRemaining() const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    void EndGame(bool bWon);
    void HandleTimeExpired();

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "1"))
    int32 KeysRequired = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "1"))
    int32 StartingLives = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float LevelSeconds = 300.f;

    /** Assign your WBP_Result child of this on BP_LavaGameMode. */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UResultWidget> ResultWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 KeysCollected = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 LivesLeft = 3;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 Score = 0;

    bool bGameOver = false;
    FTimerHandle LevelTimer;
};
