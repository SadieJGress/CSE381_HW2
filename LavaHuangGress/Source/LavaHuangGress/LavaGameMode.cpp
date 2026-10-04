#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "LavaHuangGressCharacter.h"
#include "Public/ResultWidget.h"
#include "lava.h"

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
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();

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

}

void ALavaGameMode::UpdateTimer()
{
	if (bGameOver) return;

	RemainingTime -= 1.0f;

	// Print remaining time to screen via debug message
	PRINT_LOG("Time Remaining: %.0f", RemainingTime);

	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0.0f;
		OnTimerExpired();
	}
}

void ALavaGameMode::OnTimerExpired()
{
	PRINT_LOG("Timer expired");
	EndGame(false);
}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorldTimerManager().ClearTimer(LevelTimer);
	Super::EndPlay(Reason);
}

void ALavaGameMode::ReportKeyCollected()
{
}

void ALavaGameMode::ReportLifeLost()
{
	if (bGameOver) { return; }

	ALavaHuangGressCharacter* Player = Cast<ALavaHuangGressCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Stop from triggering lost lives multiple times
	if (!Player || Player->IsHurt()) {return;}
	LivesLeft--;
	PRINT_LOG("Lives Left: %d", LivesLeft);


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

		// Show Result Screen
		if (ResultWidgetClass)
		{
			UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);
			if (ResultWidget)
			{
				ResultWidget->AddToViewport();
				ResultWidget->SetupResult(bWon);

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