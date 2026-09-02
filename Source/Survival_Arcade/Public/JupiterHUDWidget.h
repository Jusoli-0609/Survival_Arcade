#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JupiterHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class SWidget;

UCLASS(Blueprintable)
class SURVIVAL_ARCADE_API UJupiterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void RefreshHUD();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UTextBlock> WaveText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "HUD")
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	FLinearColor PrimaryTextColor = FLinearColor(0.92f, 0.96f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	FLinearColor AccentColor = FLinearColor(0.15f, 0.85f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	FLinearColor WarningColor = FLinearColor(1.0f, 0.22f, 0.12f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	float RefreshInterval = 0.1f;

private:
	void BuildDefaultLayout();
	void ApplyTextStyle(
		UTextBlock* TextBlock,
		int32 FontSize,
		const FLinearColor& Color
	) const;

	FTimerHandle HUDRefreshTimerHandle;
};
