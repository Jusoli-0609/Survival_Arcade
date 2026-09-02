#include "JupiterHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "JupiterCharacter.h"
#include "JupiterGameState.h"
#include "TimerManager.h"

TSharedRef<SWidget> UJupiterHUDWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildDefaultLayout();
	}

	return Super::RebuildWidget();
}

void UJupiterHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshHUD();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			HUDRefreshTimerHandle,
			this,
			&UJupiterHUDWidget::RefreshHUD,
			FMath::Max(RefreshInterval, 0.05f),
			true
		);
	}
}

void UJupiterHUDWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HUDRefreshTimerHandle);
	}

	Super::NativeDestruct();
}

void UJupiterHUDWidget::BuildDefaultLayout()
{
	UCanvasPanel* RootCanvas =
		WidgetTree->ConstructWidget<UCanvasPanel>(
			UCanvasPanel::StaticClass(),
			TEXT("HUDRoot")
		);
	WidgetTree->RootWidget = RootCanvas;

	UBorder* StatusBackground =
		WidgetTree->ConstructWidget<UBorder>(
			UBorder::StaticClass(),
			TEXT("StatusBackground")
		);
	StatusBackground->SetPadding(FMargin(22.0f, 14.0f));
	StatusBackground->SetBrushColor(
		FLinearColor(0.015f, 0.03f, 0.07f, 0.86f)
	);

	UCanvasPanelSlot* BackgroundSlot =
		RootCanvas->AddChildToCanvas(StatusBackground);
	BackgroundSlot->SetAnchors(
		FAnchors(0.0f, 0.0f, 1.0f, 0.0f)
	);
	BackgroundSlot->SetOffsets(FMargin(32.0f, 24.0f, 32.0f, 104.0f));

	UHorizontalBox* StatusRow =
		WidgetTree->ConstructWidget<UHorizontalBox>(
			UHorizontalBox::StaticClass(),
			TEXT("StatusRow")
		);
	StatusBackground->SetContent(StatusRow);

	UVerticalBox* ProgressGroup =
		WidgetTree->ConstructWidget<UVerticalBox>(
			UVerticalBox::StaticClass(),
			TEXT("ProgressGroup")
		);

	UHorizontalBoxSlot* ProgressSlot =
		StatusRow->AddChildToHorizontalBox(ProgressGroup);
	ProgressSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	ProgressSlot->SetVerticalAlignment(VAlign_Center);

	ScoreText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("ScoreText")
	);
	ScoreText->SetText(FText::FromString(TEXT("SCORE  0")));
	ApplyTextStyle(ScoreText, 24, AccentColor);
	ProgressGroup->AddChildToVerticalBox(ScoreText);

	LevelText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("LevelText")
	);
	LevelText->SetText(FText::FromString(TEXT("LEVEL  1 / 3")));
	ApplyTextStyle(LevelText, 17, PrimaryTextColor);
	ProgressGroup->AddChildToVerticalBox(LevelText);

	WaveText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("WaveText")
	);
	WaveText->SetText(FText::FromString(TEXT("WAVE  1 / 3")));
	ApplyTextStyle(WaveText, 17, PrimaryTextColor);
	ProgressGroup->AddChildToVerticalBox(WaveText);

	TimerText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("TimerText")
	);
	TimerText->SetText(FText::FromString(TEXT("TIME  00:00")));
	TimerText->SetJustification(ETextJustify::Center);
	ApplyTextStyle(TimerText, 32, PrimaryTextColor);

	UHorizontalBoxSlot* TimerSlot =
		StatusRow->AddChildToHorizontalBox(TimerText);
	TimerSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	TimerSlot->SetHorizontalAlignment(HAlign_Center);
	TimerSlot->SetVerticalAlignment(VAlign_Center);

	UVerticalBox* HealthGroup =
		WidgetTree->ConstructWidget<UVerticalBox>(
			UVerticalBox::StaticClass(),
			TEXT("HealthGroup")
		);

	UHorizontalBoxSlot* HealthGroupSlot =
		StatusRow->AddChildToHorizontalBox(HealthGroup);
	HealthGroupSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	HealthGroupSlot->SetHorizontalAlignment(HAlign_Fill);
	HealthGroupSlot->SetVerticalAlignment(VAlign_Center);

	HealthText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("HealthText")
	);
	HealthText->SetText(FText::FromString(TEXT("HP  100 / 100")));
	HealthText->SetJustification(ETextJustify::Right);
	ApplyTextStyle(HealthText, 20, PrimaryTextColor);
	HealthGroup->AddChildToVerticalBox(HealthText);

	USizeBox* HealthBarSize =
		WidgetTree->ConstructWidget<USizeBox>(
			USizeBox::StaticClass(),
			TEXT("HealthBarSize")
		);
	HealthBarSize->SetHeightOverride(18.0f);
	HealthBarSize->SetMinDesiredWidth(230.0f);

	UVerticalBoxSlot* HealthBarSlot =
		HealthGroup->AddChildToVerticalBox(HealthBarSize);
	HealthBarSlot->SetPadding(FMargin(0.0f, 7.0f, 0.0f, 0.0f));

	HealthBar = WidgetTree->ConstructWidget<UProgressBar>(
		UProgressBar::StaticClass(),
		TEXT("HealthBar")
	);
	HealthBar->SetPercent(1.0f);
	HealthBar->SetFillColorAndOpacity(
		FLinearColor(0.13f, 0.95f, 0.46f, 1.0f)
	);
	HealthBarSize->SetContent(HealthBar);
}

void UJupiterHUDWidget::ApplyTextStyle(
	UTextBlock* TextBlock,
	int32 FontSize,
	const FLinearColor& Color) const
{
	if (!TextBlock)
	{
		return;
	}

	FSlateFontInfo FontInfo = TextBlock->GetFont();
	FontInfo.Size = FontSize;
	FontInfo.OutlineSettings.OutlineSize = 1;
	FontInfo.OutlineSettings.OutlineColor =
		FLinearColor(0.0f, 0.0f, 0.0f, 0.85f);

	TextBlock->SetFont(FontInfo);
	TextBlock->SetColorAndOpacity(FSlateColor(Color));
	TextBlock->SetShadowOffset(FVector2D(1.5f, 1.5f));
	TextBlock->SetShadowColorAndOpacity(
		FLinearColor(0.0f, 0.0f, 0.0f, 0.75f)
	);
}

void UJupiterHUDWidget::RefreshHUD()
{
	const AJupiterGameState* JupiterGameState =
		GetWorld() ? GetWorld()->GetGameState<AJupiterGameState>() : nullptr;

	if (JupiterGameState)
	{
		if (ScoreText)
		{
			ScoreText->SetText(
				FText::FromString(
					FString::Printf(
						TEXT("SCORE  %d"),
						JupiterGameState->GetScore()
					)
				)
			);
		}

		if (LevelText)
		{
			LevelText->SetText(
				FText::FromString(
					FString::Printf(
						TEXT("LEVEL  %d / %d"),
						JupiterGameState->GetCurrentLevelNumber(),
						JupiterGameState->GetTotalLevelCount()
					)
				)
			);
		}

		if (WaveText)
		{
			WaveText->SetText(
				FText::FromString(
					FString::Printf(
						TEXT("WAVE  %d / %d"),
						JupiterGameState->GetCurrentWaveNumber(),
						JupiterGameState->MaxWavesPerLevel
					)
				)
			);
		}

		const float RemainingTime =
			JupiterGameState->GetRemainingWaveTime();
		const int32 TotalSeconds =
			FMath::Max(FMath::CeilToInt(RemainingTime), 0);
		const int32 Minutes = TotalSeconds / 60;
		const int32 Seconds = TotalSeconds % 60;

		if (TimerText)
		{
			TimerText->SetText(
				FText::FromString(
					FString::Printf(
						TEXT("TIME  %02d:%02d"),
						Minutes,
						Seconds
					)
				)
			);
			TimerText->SetColorAndOpacity(
				FSlateColor(
					RemainingTime > 0.0f && RemainingTime <= 10.0f
						? WarningColor
						: PrimaryTextColor
				)
			);
		}
	}

	const AJupiterCharacter* JupiterCharacter =
		Cast<AJupiterCharacter>(GetOwningPlayerPawn());

	if (JupiterCharacter)
	{
		const float CurrentHealth = JupiterCharacter->GetHealth();
		const float MaxHealth = JupiterCharacter->GetMaxHealth();
		const float HealthPercent =
			JupiterCharacter->GetHealthPercent();

		if (HealthText)
		{
			HealthText->SetText(
				FText::FromString(
					FString::Printf(
						TEXT("HP  %d / %d"),
						FMath::RoundToInt(CurrentHealth),
						FMath::RoundToInt(MaxHealth)
					)
				)
			);
		}

		if (HealthBar)
		{
			HealthBar->SetPercent(HealthPercent);

			const FLinearColor BarColor =
				HealthPercent <= 0.3f
					? WarningColor
					: FLinearColor(0.13f, 0.95f, 0.46f, 1.0f);
			HealthBar->SetFillColorAndOpacity(BarColor);
		}
	}
}
