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

    DrawText(FString::Printf(TEXT("Lives: %d"), GM->GetLivesLeft()), FLinearColor::Red, 40, 40, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("Keys: %d / %d"), GM->GetKeysCollected(), GM->GetKeysRequired()), FLinearColor::Yellow, 40, 80, nullptr, 2.f);

    const float TimeLeft = FMath::Max(0.f, GM->GetTimeRemaining());
    const int32 TMin = FMath::FloorToInt(TimeLeft / 60.f);
    const int32 TSec = FMath::FloorToInt(FMath::Fmod(TimeLeft, 60.f));
    DrawText(FString::Printf(TEXT("Game over in: %d:%02d"), TMin, TSec), FLinearColor::Red, 40, 120, nullptr, 2.f);

    // Lava Bar
    const float Rise = FMath::Max(0.f, Lava->GetRiseHeight());
    const float Roof = Lava->GetBuildingHeight();
    const float Frac = FMath::Clamp(Rise / Roof, 0.f, 1.f);

    const float BarW = 28.f, BarH = Canvas->SizeY * 0.6f, BarX = Canvas->SizeX - 90.f, BarY = (Canvas->SizeY - BarH) * 0.5f, FillY = BarY + BarH * (1.f - Frac);
    DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.6f), BarX - 2.f, BarY - 2.f, BarW + 4.f, BarH + 4.f);
    DrawRect(FLinearColor(1.f, 0.35f, 0.f, 1.f), BarX, FillY, BarW, BarY + BarH - FillY);

    DrawText(FString::Printf(TEXT("%.0f m"), Roof / 100.f), FLinearColor::White, BarX - 10.f, BarY - 35.f, nullptr, 2.f);
    DrawText(FString::Printf(TEXT("%.0f m"), Rise / 100.f), FLinearColor::Red, BarX - 80.f, FillY - 10.f, nullptr, 2.f);

    if (FlashAlpha > 0.f)
    {
        DrawRect(FLinearColor(1.f, 0.f, 0.f, FlashAlpha), 0, 0, Canvas->SizeX, Canvas->SizeY);
        FlashAlpha = FMath::Max(0.f, FlashAlpha - GetWorld()->GetDeltaSeconds());
    }
}