#include "JupiterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "JupiterGameInstance.h"
#include "CoinItem.h"

AJupiterGameState::AJupiterGameState()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AJupiterGameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
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
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UJupiterGameInstance* JupiterGameInstance = Cast<UJupiterGameInstance>(GameInstance);
		if (JupiterGameInstance)
		{
			JupiterGameInstance->AddToScore(Amount);
		}
	}
}

void AJupiterGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UJupiterGameInstance* JupiterGameInstance = Cast<UJupiterGameInstance>(GameInstance);
		if (JupiterGameInstance)
		{
			CurrentLevelIndex = JupiterGameInstance->CurrentLevelIndex;
		}
	}
	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// 30초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AJupiterGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
		CurrentLevelIndex + 1,
		SpawnedCoinCount);
}

void AJupiterGameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AJupiterGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);

		// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void AJupiterGameState::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 다음 레벨 인덱스로
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UJupiterGameInstance* JupiterGameInstance = Cast<UJupiterGameInstance>(GameInstance);
		if (JupiterGameInstance)
		{
			JupiterGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	// 레벨 맵 이름이 있다면 해당 맵 불러오기
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		// 맵 이름이 없으면 게임오버
		OnGameOver();
	}
}

void AJupiterGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
	// 여기서 UI를 띄운다거나, 재시작 기능을 넣을 수도 있음
}