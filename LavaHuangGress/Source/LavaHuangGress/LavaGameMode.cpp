#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "LavaHuangGressCharacter.h"
#include "Public/ResultWidget.h"

ALavaGameMode::ALavaGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	LivesLeft = StartingLives;
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();
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
	if (bGameOver) return;

	LivesLeft--;

	if (LivesLeft <= 0)
	{
		EndGame(false); // Game over rule
	}
	else
	{
		// GameMode delegates the physical respawn to the character class
		if (ALavaHuangGressCharacter* Player = Cast<ALavaHuangGressCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		{
			Player->RespawnAtSurface();
		}
	}
}

void ALavaGameMode::ReportHatchReached()
{
}

float ALavaGameMode::GetTimeRemaining() const
{
	return GetWorldTimerManager().GetTimerRemaining(LevelTimer);
}

void ALavaGameMode::EndGame(bool bWon)
{
}

void ALavaGameMode::HandleTimeExpired()
{
}