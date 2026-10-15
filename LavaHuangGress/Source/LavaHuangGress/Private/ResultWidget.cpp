// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LavaGameInstance.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click event
	if (PlayAgainButton)
	{
		PlayAgainButton->OnClicked.AddDynamic(this, &UResultWidget::OnPlayAgainClicked);
	}
}

void UResultWidget::SetupResult(bool IsWin, int32 FinScore)
{
	if (ResultText)
	{
		if (IsWin)
		{
			ResultText->SetText(FText::FromString(TEXT("YOU WIN")));
			ResultText->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f)));
		}
		else
		{
			ResultText->SetText(FText::FromString(TEXT("GAME OVER")));
			ResultText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
		}
	}

	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), FinScore)));
	}
}

void UResultWidget::OnPlayAgainClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		// Restore mouse cursor and input mode to normal gameplay
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}

	// Reset the score and building index
	if (ULavaGameInstance* GameInstance = Cast<ULavaGameInstance>(GetGameInstance()))
	{
		GameInstance->ResetRun();
	}

	// Restart level  
	// FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName("Habitat67Basic"));
}

