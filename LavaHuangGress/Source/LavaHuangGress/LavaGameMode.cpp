#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "LavaHuangGressCharacter.h"
#include "Public/ResultWidget.h"
#include "Lava.h"
#include "LavaHUD.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "LavaGameInstance.h"

#define PRINT_LOG(Format, ...) \
    do { \
        FString _msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
        UE_LOG(LogTemp, Warning, TEXT("%s"), *_msg); \
        if (GEngine) { \
            GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, _msg); \
        } \
    } while(0)

ALavaGameMode::ALavaGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	LivesLeft = StartingLives;
	HUDClass = ALavaHUD::StaticClass();
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Figuring which level is starting level for testing purposes
	if (ULavaGameInstance* GameInst = Cast<ULavaGameInstance>(GetGameInstance()))
	{
		FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this);

		if (CurrentMap.Contains(TEXT("StallerQuest")))
		{
			GameInst->BuildingIndex = 1;
		}
		else if (CurrentMap.Contains(TEXT("Habitat67Basic")))
		{
			GameInst->BuildingIndex = 0;
		}
	}

	// Timer funciton
	RemainingTime = MaxTimer;
	// Start a timer to call updatetimer ever second
	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&ALavaGameMode::UpdateTimer,
		1.0f,
		true
	);

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
		EnableInput(PC);
		if (InputComponent) {
			InputComponent->BindKey(EKeys::L, IE_Pressed, this, &ALavaGameMode::DebugSpeedUpLava);
			InputComponent->BindKey(EKeys::K, IE_Pressed, this, &ALavaGameMode::DebugGrantAllKeys);
			InputComponent->BindKey(EKeys::J, IE_Pressed, this, &ALavaGameMode::DebugJumpToLevel2);
		}
	}

}

void ALavaGameMode::UpdateTimer()
{
	if (bGameOver) return;

	RemainingTime -= 1.0f;

	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0.0f;
		OnTimerExpired();
	}
}

void ALavaGameMode::OnTimerExpired()
{
	EndGame(false);
}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorldTimerManager().ClearTimer(LevelTimer);
	Super::EndPlay(Reason);
}

void ALavaGameMode::ReportKeyCollected()
{
	if (bGameOver) return;
	KeysCollected++;
	Score += 200;
}

void ALavaGameMode::ReportLifeLost()
{
	if (bGameOver) { return; }

	ALavaHuangGressCharacter* Player = Cast<ALavaHuangGressCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Stop from triggering lost lives multiple times
	if (!Player || Player->IsHurt()) {return;}
	LivesLeft--;

	// Lower score
	Score -= 100;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ALavaHUD* H = Cast<ALavaHUD>(PC->GetHUD()))
		{
			H->TriggerDamageFlash();
		}
	}

	// Drop lava level
	if (ALava* LavaActor = Cast<ALava>(UGameplayStatics::GetActorOfClass(this, ALava::StaticClass())))
	{
		LavaActor->DropLavaLevel(200.0f);
	}
	// Respawn player
	Player->LavaHurt();

	if (LivesLeft <= 0)
	{
		EndGame(false); // Game over rule
	}
}

void ALavaGameMode::ReportHatchReached()
{
	if (bGameOver) return;

	// Calculate final score before a transition
	int32 TimeValue = FMath::FloorToInt(FMath::Max(0.0f, RemainingTime));
	Score += TimeValue;

	if (ULavaGameInstance* GameInstance = Cast<ULavaGameInstance>(GetGameInstance()))
	{	
		// Save score to total
		GameInstance->AddScore(Score);

		if (GameInstance->BuildingIndex == 0)
		{
			// Move to next building
			GameInstance->BuildingIndex = 1;
			UGameplayStatics::OpenLevel(this, FName("StallerQuest"));
			return;
		}
		else
		{
			// Game completed
			EndGame(true);
		}

	}

}

float ALavaGameMode::GetTimeRemaining() const
{
	return RemainingTime;
}

void ALavaGameMode::EndGame(bool bWon)
{
	{
		if (bGameOver) return;

		bGameOver = true;

		// Stop timer
		GetWorldTimerManager().ClearTimer(LevelTimer);


		// get final score for screen 
		int32 FinalScore = Score;
		if (ULavaGameInstance* GameInstance = Cast<ULavaGameInstance>(GetGameInstance()))
		{
			if (!bWon)
			{
				GameInstance->AddScore(Score);
			}
			FinalScore = GameInstance->Score;
		}

		// Show Result Screen
		if (ResultWidgetClass)
		{
			UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);
			if (ResultWidget)
			{
				ResultWidget->AddToViewport();
				ResultWidget->SetupResult(bWon, FinalScore);

				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (PC)
				{
					PC->SetShowMouseCursor(true);
					PC->SetInputMode(FInputModeUIOnly());
				}
			}
		}
	}
}

void ALavaGameMode::HandleTimeExpired()
{
}

void ALavaGameMode::DebugSpeedUpLava()
{
	if (ALava* LavaActor = Cast<ALava>(UGameplayStatics::GetActorOfClass(this, ALava::StaticClass())))
	{
		LavaActor->ToggleDebugSpeed();
	}
}

void ALavaGameMode::DebugGrantAllKeys()
{
	if (bGameOver) return;
	const int32 Missing = FMath::Max(0, KeysRequired - KeysCollected);
	KeysCollected = KeysRequired;
	Score += 200 * Missing;
}

void ALavaGameMode::DebugJumpToLevel2()
{
	if (ULavaGameInstance* GameInstance = Cast<ULavaGameInstance>(GetGameInstance()))
	{
		GameInstance->Score = 750;
		GameInstance->BuildingIndex = 1;
		UGameplayStatics::OpenLevel(this, FName("StallerQuest"));
	}
}