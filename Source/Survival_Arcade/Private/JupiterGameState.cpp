#include "JupiterGameState.h"

AJupiterGameState::AJupiterGameState()
{
    Score = 0;
}

int32 AJupiterGameState::GetScore() const
{
    return Score;
}

void AJupiterGameState::AddScore(int32 Amount)
{
    Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}