#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JupiterGameState.generated.h"

USTRUCT(BlueprintType)
struct FJupiterWaveData
{
	GENERATED_BODY()

public:
	// 0: BasicLevel, 1: IntermediateLevel, 2: AdvancedLevel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 LevelIndex = 0;

	// 에디터에서 읽기 쉽도록 1부터 시작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 WaveNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ItemSpawnCount = 20;
};

UCLASS()
class SURVIVAL_ARCADE_API AJupiterGameState : public AGameState
{
	GENERATED_BODY()

public:
	AJupiterGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FJupiterWaveData> WaveSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float CurrentWaveDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWavesPerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	bool bWaveActive;

	FTimerHandle WaveTimerHandle;

	TArray<TWeakObjectPtr<AActor>> SpawnedWaveItems;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void StartWave();
	void OnWaveTimeUp();
	void EndWave();
	void CleanupWaveItems();
	void OnCoinCollected();
	void EndLevel();

	const FJupiterWaveData* FindCurrentWaveData() const;
};
