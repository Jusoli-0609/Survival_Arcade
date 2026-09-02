#include "JupiterGameInstance.h"

UJupiterGameInstance::UJupiterGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UJupiterGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

void UJupiterGameInstance::ResetGameData()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}