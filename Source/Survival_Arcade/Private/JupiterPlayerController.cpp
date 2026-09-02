#include "JupiterPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "JupiterGameInstance.h"
#include "JupiterHUDWidget.h"
#include "JupiterMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AJupiterPlayerController::AJupiterPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr)
{
}

void AJupiterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	if (bShowMainMenuOnBeginPlay)
	{
		ShowMainMenu();
	}
	else
	{
		ShowHUD();
	}
}

void AJupiterPlayerController::ShowHUD()
{
	if (ActiveMenuWidget)
	{
		ActiveMenuWidget->RemoveFromParent();
		ActiveMenuWidget = nullptr;
	}

	if (!HUDWidget)
	{
		TSubclassOf<UUserWidget> ResolvedClass = HUDWidgetClass;

		if (!ResolvedClass ||
			!ResolvedClass->IsChildOf(UJupiterHUDWidget::StaticClass()))
		{
			if (ResolvedClass)
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("HUDWidgetClass must inherit from JupiterHUDWidget. Using the native fallback HUD.")
				);
			}

			ResolvedClass = UJupiterHUDWidget::StaticClass();
		}

		HUDWidget = CreateWidget<UUserWidget>(this, ResolvedClass);
	}

	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
	}

	SetPause(false);
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void AJupiterPlayerController::ShowMainMenu()
{
	ShowMenu(MainMenuWidgetClass, EJupiterMenuMode::MainMenu);
}

void AJupiterPlayerController::ShowGameOverMenu()
{
	ShowMenu(GameOverWidgetClass, EJupiterMenuMode::GameOver);
}

void AJupiterPlayerController::ShowMenu(
	TSubclassOf<UUserWidget> RequestedClass,
	EJupiterMenuMode MenuMode)
{
	if (ActiveMenuWidget)
	{
		ActiveMenuWidget->RemoveFromParent();
		ActiveMenuWidget = nullptr;
	}

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
	}

	TSubclassOf<UUserWidget> ResolvedClass = RequestedClass;

	if (!ResolvedClass ||
		!ResolvedClass->IsChildOf(UJupiterMenuWidget::StaticClass()))
	{
		if (ResolvedClass)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Menu widget class must inherit from JupiterMenuWidget. Using the native fallback menu.")
			);
		}

		ResolvedClass = UJupiterMenuWidget::StaticClass();
	}

	ActiveMenuWidget = CreateWidget<UUserWidget>(this, ResolvedClass);

	if (!ActiveMenuWidget)
	{
		return;
	}

	if (UJupiterMenuWidget* JupiterMenu =
		Cast<UJupiterMenuWidget>(ActiveMenuWidget))
	{
		JupiterMenu->SetMenuMode(MenuMode);
	}

	ActiveMenuWidget->AddToViewport(100);
	SetPause(true);

	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ActiveMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AJupiterPlayerController::HideAllWidgets()
{
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}

	if (ActiveMenuWidget)
	{
		ActiveMenuWidget->RemoveFromParent();
		ActiveMenuWidget = nullptr;
	}
}

void AJupiterPlayerController::StartNewGame()
{
	if (UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		JupiterGameInstance->ResetGameData();
	}

	SetPause(false);

	if (!FirstGameLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, FirstGameLevelName);
	}
}

void AJupiterPlayerController::RestartGame()
{
	StartNewGame();
}

void AJupiterPlayerController::ReturnToMainMenu()
{
	if (UJupiterGameInstance* JupiterGameInstance =
		Cast<UJupiterGameInstance>(GetGameInstance()))
	{
		JupiterGameInstance->ResetGameData();
	}

	SetPause(false);

	if (!MainMenuLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName);
		return;
	}

	ShowMainMenu();
}

void AJupiterPlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		this,
		this,
		EQuitPreference::Quit,
		false
	);
}
