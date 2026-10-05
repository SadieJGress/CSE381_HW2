// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class LAVAHUANGGRESS_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// Match to the UI elements in Result
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText; 

	UPROPERTY(meta = (BindWidget))
	UButton* PlayAgainButton;

	UFUNCTION()
	void OnPlayAgainClicked();

public:
	// Need to call from lavagamemode
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupResult(bool bIsWin, int32 FinScore);
};