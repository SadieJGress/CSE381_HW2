// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click event
	if (PlayAgainButton)
	{
		PlayAgainButton->OnClicked.AddDynamic(this, &UResultWidget::OnPlayAgainClicked);
	}
}

void UResultWidget::SetupResult(bool bIsWin)
{
	if (ResultText)
	{
		if (bIsWin)
		{
			ResultText->SetText(FText::FromString(TEXT("YOU WIN!")));
		}
		else
		{
			ResultText->SetText(FText::FromString(TEXT("GAME OVER!")));
		}
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

	// Restart current level
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

