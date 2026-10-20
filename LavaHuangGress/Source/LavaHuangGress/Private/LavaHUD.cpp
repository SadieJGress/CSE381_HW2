#include "LavaHUD.h"
#include "LavaGameMode.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "LavaHuangGressCharacter.h"
#include "InventoryComponent.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

ALavaHUD::ALavaHUD()
{
    // Load default icons from Content/UI/ automatically
    static ConstructorHelpers::FObjectFinder<UTexture2D> CushionTexObj(TEXT("/Game/UI/cushion"));
    if (CushionTexObj.Succeeded()) CushionPicture = CushionTexObj.Object;

    static ConstructorHelpers::FObjectFinder<UTexture2D> ExtTexObj(TEXT("/Game/UI/extinguisher"));
    if (ExtTexObj.Succeeded()) ExtinguisherPicture = ExtTexObj.Object;
}

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

	//Inventory
	ALavaHuangGressCharacter* Player = Cast<ALavaHuangGressCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player && Player->InventoryComponent)
	{
		const int32 Cushions = Player->InventoryComponent->GetCount(EItemSlot::Cushion);
		const int32 Extinguishers = Player->InventoryComponent->GetCount(EItemSlot::Extinguisher);
		const EItemSlot ActiveSlot = Player->CurrentlyEquippedSlot;

		// Bunch of values
		const float BoxW = 120.f;
		const float BoxH = 60.f;
		const float IconSize = 40.f;
		const float Spacing = 15.f;

		const float TotalW = (BoxW * 2.f) + Spacing;
		const float StartX = (Canvas->SizeX - TotalW) * 0.5f;
		const float StartY = Canvas->SizeY - BoxH - 40.f;

		// Cushion Slot
		{
			const bool bIsActive = (ActiveSlot == EItemSlot::Cushion);
			const float X = StartX;
			const float Y = StartY;

			// Highlight border if selected
			if (bIsActive)
			{
				DrawRect(FLinearColor::Green, X - 3.f, Y - 3.f, BoxW + 6.f, BoxH + 6.f);
			}

			// Card background
			DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), X, Y, BoxW, BoxH);

			// Draw Cushion Icon Texture
			if (CushionPicture)
			{
				DrawTexture(CushionPicture, X + 10.f, Y + (BoxH - IconSize) * 0.5f, IconSize, IconSize, 0.f, 0.f, 1.f, 1.f, FLinearColor::White);
			}

			// Item count
			DrawText(FString::Printf(TEXT("%d / 3"), Cushions), bIsActive ? FLinearColor::Green : FLinearColor::White, X + 60.f, Y + 18.f, nullptr, 1.6f);
		}

		// Extinguisher
		{
			const bool bIsActive = (ActiveSlot == EItemSlot::Extinguisher);
			const float X = StartX + BoxW + Spacing;
			const float Y = StartY;

			if (bIsActive)
			{
				DrawRect(FLinearColor::Green, X - 3.f, Y - 3.f, BoxW + 6.f, BoxH + 6.f);
			}

			DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), X, Y, BoxW, BoxH);

			if (ExtinguisherPicture)
			{
				DrawTexture(ExtinguisherPicture, X + 10.f, Y + (BoxH - IconSize) * 0.5f, IconSize, IconSize, 0.f, 0.f, 1.f, 1.f, FLinearColor::White);
			}

			DrawText(FString::Printf(TEXT("%d / 2"), Extinguishers), bIsActive ? FLinearColor::Green : FLinearColor::White, X + 60.f, Y + 18.f, nullptr, 1.6f);
		}

		// Crosshair
		if (ActiveSlot == EItemSlot::Extinguisher)
		{
			const float CenterX = Canvas->SizeX * 0.5f;
			const float CenterY = Canvas->SizeY * 0.5f;
			const float CrosshairSize = 12.f;

			DrawLine(CenterX - CrosshairSize, CenterY, CenterX + CrosshairSize, CenterY, FLinearColor::Green, 2.f);
			DrawLine(CenterX, CenterY - CrosshairSize, CenterX, CenterY + CrosshairSize, FLinearColor::Green, 2.f);
		}
	}

    if (FlashAlpha > 0.f)
    {
        DrawRect(FLinearColor(1.f, 0.f, 0.f, FlashAlpha), 0, 0, Canvas->SizeX, Canvas->SizeY);
        FlashAlpha = FMath::Max(0.f, FlashAlpha - GetWorld()->GetDeltaSeconds());
    }
}