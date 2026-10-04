#include "LavaHUD.h"
#include "LavaGameMode.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void ALavaHUD::DrawHUD()
{
    Super::DrawHUD();
    ALavaGameMode* GM = GetWorld()->GetAuthGameMode<ALavaGameMode>();
    ALava* Lava = Cast<ALava>(UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass()));
    if (!GM || !Lava) return;

    const float Time = FMath::Max(0.f, GM->GetTimeRemaining());
    const int32 Min = FMath::FloorToInt(Time / 60.f);
    const int32 Sec = FMath::FloorToInt(FMath::Fmod(Time, 60.f));

    DrawText(FString::Printf(TEXT("Lives: %d"), GM->GetLivesLeft()), FLinearColor::Red, 40, 40, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("Time: %d:%02d"), Min, Sec), FLinearColor::White, 40, 80, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("Keys: %d / %d"), GM->GetKeysCollected(), GM->GetKeysRequired()), FLinearColor::Yellow, 40, 120, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("Lava: %.0f m"), Lava->GetRiseHeight() / 100.f), FLinearColor(1.f, 0.4f, 0.f), 40, 160, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("Score: %d"), GM->GetScore()), FLinearColor::White, 40, 200, nullptr, 2.f);

    if (FlashAlpha > 0.f)
    {
        DrawRect(FLinearColor(1.f, 0.f, 0.f, FlashAlpha), 0, 0, Canvas->SizeX, Canvas->SizeY);
        FlashAlpha = FMath::Max(0.f, FlashAlpha - GetWorld()->GetDeltaSeconds());
    }
}