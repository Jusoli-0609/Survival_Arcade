#include "JupiterMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Brushes/SlateColorBrush.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "JupiterPlayerController.h"

TSharedRef<SWidget> UJupiterMenuWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildDefaultLayout();
	}

	return Super::RebuildWidget();
}

void UJupiterMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtonEvents();
	RefreshMenu();
}

void UJupiterMenuWidget::SetMenuMode(EJupiterMenuMode NewMenuMode)
{
	MenuMode = NewMenuMode;
	RefreshMenu();
}

void UJupiterMenuWidget::BuildDefaultLayout()
{
	UCanvasPanel* RootCanvas =
		WidgetTree->ConstructWidget<UCanvasPanel>(
			UCanvasPanel::StaticClass(),
			TEXT("MenuRoot")
		);
	WidgetTree->RootWidget = RootCanvas;

	UBackgroundBlur* BackgroundBlur =
		WidgetTree->ConstructWidget<UBackgroundBlur>(
			UBackgroundBlur::StaticClass(),
			TEXT("BackgroundBlur")
		);
	BackgroundBlur->SetBlurStrength(14.0f);
	BackgroundBlur->SetBlurRadius(24);

	UCanvasPanelSlot* BlurSlot =
		RootCanvas->AddChildToCanvas(BackgroundBlur);
	BlurSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	BlurSlot->SetOffsets(FMargin(0.0f));

	UBorder* DimLayer =
		WidgetTree->ConstructWidget<UBorder>(
			UBorder::StaticClass(),
			TEXT("DimLayer")
		);
	DimLayer->SetBrushColor(FLinearColor(0.0f, 0.01f, 0.04f, 0.58f));

	UCanvasPanelSlot* DimSlot =
		RootCanvas->AddChildToCanvas(DimLayer);
	DimSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	DimSlot->SetOffsets(FMargin(0.0f));

	UBorder* MenuCard =
		WidgetTree->ConstructWidget<UBorder>(
			UBorder::StaticClass(),
			TEXT("MenuCard")
		);
	MenuCard->SetPadding(FMargin(48.0f, 42.0f));
	MenuCard->SetBrushColor(
		FLinearColor(0.015f, 0.035f, 0.09f, 0.96f)
	);

	UCanvasPanelSlot* CardSlot =
		RootCanvas->AddChildToCanvas(MenuCard);
	CardSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	CardSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	CardSlot->SetPosition(FVector2D::ZeroVector);
	CardSlot->SetSize(FVector2D(540.0f, 500.0f));

	UVerticalBox* MenuColumn =
		WidgetTree->ConstructWidget<UVerticalBox>(
			UVerticalBox::StaticClass(),
			TEXT("MenuColumn")
		);
	MenuCard->SetContent(MenuColumn);

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("TitleText")
	);
	TitleText->SetJustification(ETextJustify::Center);
	FSlateFontInfo TitleFont = TitleText->GetFont();
	TitleFont.Size = 42;
	TitleFont.OutlineSettings.OutlineSize = 2;
	TitleFont.OutlineSettings.OutlineColor =
		FLinearColor(0.0f, 0.0f, 0.0f, 0.9f);
	TitleText->SetFont(TitleFont);
	TitleText->SetColorAndOpacity(
		FSlateColor(FLinearColor(0.15f, 0.85f, 1.0f, 1.0f))
	);

	UVerticalBoxSlot* TitleSlot =
		MenuColumn->AddChildToVerticalBox(TitleText);
	TitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));

	SubtitleText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("SubtitleText")
	);
	SubtitleText->SetJustification(ETextJustify::Center);
	FSlateFontInfo SubtitleFont = SubtitleText->GetFont();
	SubtitleFont.Size = 17;
	SubtitleText->SetFont(SubtitleFont);
	SubtitleText->SetColorAndOpacity(
		FSlateColor(FLinearColor(0.72f, 0.78f, 0.88f, 1.0f))
	);
	SubtitleText->SetAutoWrapText(true);

	UVerticalBoxSlot* SubtitleSlot =
		MenuColumn->AddChildToVerticalBox(SubtitleText);
	SubtitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 36.0f));

	StartButton = CreateStyledButton(
		TEXT("StartButton"),
		TEXT("START GAME")
	);
	UVerticalBoxSlot* StartSlot =
		MenuColumn->AddChildToVerticalBox(StartButton);
	StartSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));

	RestartButton = CreateStyledButton(
		TEXT("RestartButton"),
		TEXT("RESTART")
	);
	UVerticalBoxSlot* RestartSlot =
		MenuColumn->AddChildToVerticalBox(RestartButton);
	RestartSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));

	MainMenuButton = CreateStyledButton(
		TEXT("MainMenuButton"),
		TEXT("MAIN MENU")
	);
	UVerticalBoxSlot* MainMenuSlot =
		MenuColumn->AddChildToVerticalBox(MainMenuButton);
	MainMenuSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));

	QuitButton = CreateStyledButton(
		TEXT("QuitButton"),
		TEXT("QUIT GAME")
	);
	MenuColumn->AddChildToVerticalBox(QuitButton);
}

UButton* UJupiterMenuWidget::CreateStyledButton(
	const FName& ButtonName,
	const FString& Label)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>(
		UButton::StaticClass(),
		ButtonName
	);

	FButtonStyle ButtonStyle;
	ButtonStyle.SetNormal(
		FSlateColorBrush(FLinearColor(0.04f, 0.12f, 0.24f, 1.0f))
	);
	ButtonStyle.SetHovered(
		FSlateColorBrush(FLinearColor(0.05f, 0.55f, 0.72f, 1.0f))
	);
	ButtonStyle.SetPressed(
		FSlateColorBrush(FLinearColor(0.02f, 0.31f, 0.46f, 1.0f))
	);
	ButtonStyle.SetDisabled(
		FSlateColorBrush(FLinearColor(0.12f, 0.14f, 0.18f, 0.6f))
	);
	ButtonStyle.SetNormalPadding(FMargin(18.0f, 14.0f));
	ButtonStyle.SetPressedPadding(FMargin(18.0f, 16.0f, 18.0f, 12.0f));
	Button->SetStyle(ButtonStyle);

	UTextBlock* LabelText =
		WidgetTree->ConstructWidget<UTextBlock>(
			UTextBlock::StaticClass(),
			FName(*(ButtonName.ToString() + TEXT("_Label")))
		);
	LabelText->SetText(FText::FromString(Label));
	LabelText->SetJustification(ETextJustify::Center);

	FSlateFontInfo LabelFont = LabelText->GetFont();
	LabelFont.Size = 22;
	LabelFont.OutlineSettings.OutlineSize = 1;
	LabelText->SetFont(LabelFont);
	LabelText->SetColorAndOpacity(
		FSlateColor(FLinearColor(0.95f, 0.98f, 1.0f, 1.0f))
	);

	Button->SetContent(LabelText);
	return Button;
}

void UJupiterMenuWidget::BindButtonEvents()
{
	if (StartButton)
	{
		StartButton->OnClicked.AddUniqueDynamic(
			this,
			&UJupiterMenuWidget::HandleStartClicked
		);
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddUniqueDynamic(
			this,
			&UJupiterMenuWidget::HandleRestartClicked
		);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddUniqueDynamic(
			this,
			&UJupiterMenuWidget::HandleMainMenuClicked
		);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(
			this,
			&UJupiterMenuWidget::HandleQuitClicked
		);
	}
}

void UJupiterMenuWidget::RefreshMenu()
{
	const bool bIsMainMenu = MenuMode == EJupiterMenuMode::MainMenu;

	if (TitleText)
	{
		TitleText->SetText(
			FText::FromString(
				bIsMainMenu
					? TEXT("SURVIVAL ARCADE")
					: TEXT("GAME OVER")
			)
		);
	}

	if (SubtitleText)
	{
		SubtitleText->SetText(
			FText::FromString(
				bIsMainMenu
					? TEXT("Clear every wave and chase the high score.")
					: TEXT("Try again or return to the main menu.")
			)
		);
	}

	if (StartButton)
	{
		StartButton->SetVisibility(
			bIsMainMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
		);
	}

	if (QuitButton)
	{
		QuitButton->SetVisibility(
			bIsMainMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
		);
	}

	if (RestartButton)
	{
		RestartButton->SetVisibility(
			bIsMainMenu ? ESlateVisibility::Collapsed : ESlateVisibility::Visible
		);
	}

	if (MainMenuButton)
	{
		MainMenuButton->SetVisibility(
			bIsMainMenu ? ESlateVisibility::Collapsed : ESlateVisibility::Visible
		);
	}
}

void UJupiterMenuWidget::HandleStartClicked()
{
	if (AJupiterPlayerController* PlayerController =
		Cast<AJupiterPlayerController>(GetOwningPlayer()))
	{
		PlayerController->StartNewGame();
	}
}

void UJupiterMenuWidget::HandleRestartClicked()
{
	if (AJupiterPlayerController* PlayerController =
		Cast<AJupiterPlayerController>(GetOwningPlayer()))
	{
		PlayerController->RestartGame();
	}
}

void UJupiterMenuWidget::HandleMainMenuClicked()
{
	if (AJupiterPlayerController* PlayerController =
		Cast<AJupiterPlayerController>(GetOwningPlayer()))
	{
		PlayerController->ReturnToMainMenu();
	}
}

void UJupiterMenuWidget::HandleQuitClicked()
{
	if (AJupiterPlayerController* PlayerController =
		Cast<AJupiterPlayerController>(GetOwningPlayer()))
	{
		PlayerController->QuitGame();
	}
}
