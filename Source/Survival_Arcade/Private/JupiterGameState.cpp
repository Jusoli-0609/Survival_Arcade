#include "JupiterGameState.h"
#include "CoinItem.h"
#include "Engine/Engine.h"
#include "JupiterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"

AJupiterGameState::AJupiterGameState()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
	CurrentWaveDuration = 0.0f;
	CurrentWaveItemCount = 0;
	MaxWavesPerLevel = 3;
	bWaveActive = false;

	LevelMapNames =
	{
		FName(TEXT("BasicLevel")),
		FName(TEXT("IntermediateLevel")),
		FName(TEXT("AdvancedLevel"))
	};

	const auto AddWave = [this](
		int32 LevelIndex,
		int32 WaveNumber,
		float WaveDuration,
		int32 ItemSpawnCount)
	{
		FJupiterWaveData WaveData;
		WaveData.LevelIndex = LevelIndex;
		WaveData.WaveNumber = WaveNumber;
		WaveData.WaveDuration = WaveDuration;
		WaveData.ItemSpawnCount = ItemSpawnCount;
		WaveSettings.Add(WaveData);
	};

	AddWave(0, 1, 30.0f, 20);
	AddWave(0, 2, 25.0f, 25);
	AddWave(0, 3, 20.0f, 30);

	AddWave(1, 1, 30.0f, 25);
	AddWave(1, 2, 25.0f, 30);
	AddWave(1, 3, 20.0f, 35);

	AddWave(2, 1, 30.0f, 30);
	AddWave(2, 2, 25.0f, 35);
	AddWave(2, 3, 20.0f, 40);
}

void AJupiterGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

int32 AJupiterGameState::GetScore() const
{
	if (const UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		return JupiterGameInstance->TotalScore;
	}

	return 0;
}

void AJupiterGameState::AddScore(int32 Amount)
{
	if (UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		JupiterGameInstance->AddToScore(Amount);
	}
}

void AJupiterGameState::StartLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	bWaveActive = false;
	CleanupWaveItems();

	if (UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		CurrentLevelIndex = JupiterGameInstance->CurrentLevelIndex;
	}

	CurrentWaveIndex = 0;
	StartWave();
}

const FJupiterWaveData* AJupiterGameState::FindCurrentWaveData() const
{
	for (const FJupiterWaveData& WaveData : WaveSettings)
	{
		const bool bSameLevel =
			WaveData.LevelIndex == CurrentLevelIndex;
		const bool bSameWave =
			WaveData.WaveNumber == CurrentWaveIndex + 1;

		if (bSameLevel && bSameWave)
		{
			return &WaveData;
		}
	}

	return nullptr;
}

void AJupiterGameState::StartWave()
{
	const FJupiterWaveData* WaveData = FindCurrentWaveData();

	if (!WaveData)
	{
		bWaveActive = false;

		UE_LOG(
			LogTemp,
			Error,
			TEXT("Level %d, Wave %d 설정을 찾지 못했습니다."),
			CurrentLevelIndex + 1,
			CurrentWaveIndex + 1
		);

		return;
	}

	CurrentWaveDuration = FMath::Max(WaveData->WaveDuration, 0.1f);
	CurrentWaveItemCount = FMath::Max(WaveData->ItemSpawnCount, 0);

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	SpawnedWaveItems.Empty();

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawnVolume::StaticClass(),
		FoundVolumes
	);

	if (FoundVolumes.IsEmpty())
	{
		bWaveActive = false;
		UE_LOG(LogTemp, Error, TEXT("SpawnVolume을 찾지 못했습니다."));
		return;
	}

	for (int32 i = 0; i < CurrentWaveItemCount; i++)
	{
		const int32 VolumeIndex = i % FoundVolumes.Num();
		ASpawnVolume* SpawnVolume =
			Cast<ASpawnVolume>(FoundVolumes[VolumeIndex]);

		if (!SpawnVolume)
		{
			continue;
		}

		AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

		if (!SpawnedActor)
		{
			continue;
		}

		SpawnedWaveItems.Add(SpawnedActor);

		if (SpawnedActor->IsA(ACoinItem::StaticClass()))
		{
			SpawnedCoinCount++;
		}
	}

	bWaveActive = true;

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AJupiterGameState::OnWaveTimeUp,
		CurrentWaveDuration,
		false
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Level %d - Wave %d Start! Duration: %.1f, Items: %d, Coins: %d"),
		CurrentLevelIndex + 1,
		CurrentWaveIndex + 1,
		CurrentWaveDuration,
		CurrentWaveItemCount,
		SpawnedCoinCount
	);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(
				TEXT("Level %d - Wave %d 시작!"),
				CurrentLevelIndex + 1,
				CurrentWaveIndex + 1
			)
		);
	}
}

void AJupiterGameState::OnWaveTimeUp()
{
	EndWave();
}

void AJupiterGameState::EndWave()
{
	if (!bWaveActive)
	{
		return;
	}

	bWaveActive = false;
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CleanupWaveItems();

	CurrentWaveIndex++;

	if (CurrentWaveIndex < MaxWavesPerLevel)
	{
		StartWave();
	}
	else
	{
		EndLevel();
	}
}

void AJupiterGameState::CleanupWaveItems()
{
	for (TWeakObjectPtr<AActor>& Item : SpawnedWaveItems)
	{
		if (AActor* ItemActor = Item.Get())
		{
			ItemActor->Destroy();
		}
	}

	SpawnedWaveItems.Empty();
}

void AJupiterGameState::OnCoinCollected()
{
	if (!bWaveActive)
	{
		return;
	}

	CollectedCoinCount++;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount
	);

	if (SpawnedCoinCount > 0 &&
		CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void AJupiterGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	bWaveActive = false;
	CleanupWaveItems();

	CurrentLevelIndex++;

	if (UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		JupiterGameInstance->CurrentLevelIndex = CurrentLevelIndex;
	}

	if (CurrentLevelIndex >= LevelMapNames.Num())
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(
			GetWorld(),
			LevelMapNames[CurrentLevelIndex]
		);
	}
	else
	{
		OnGameOver();
	}
}

void AJupiterGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}
