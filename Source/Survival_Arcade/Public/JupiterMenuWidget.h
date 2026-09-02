#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JupiterMenuWidget.generated.h"

class UButton;
class UTextBlock;
class SWidget;

UENUM(BlueprintType)
enum class EJupiterMenuMode : uint8
{
	MainMenu UMETA(DisplayName = "Main Menu"),
	GameOver UMETA(DisplayName = "Game Over")
};

UCLASS(Blueprintable)
class SURVIVAL_ARCADE_API UJupiterMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void SetMenuMode(EJupiterMenuMode NewMenuMode);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	EJupiterMenuMode MenuMode = EJupiterMenuMode::MainMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UTextBlock> SubtitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UButton> StartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Menu")
	TObjectPtr<UButton> QuitButton;

private:
	void BuildDefaultLayout();
	void RefreshMenu();
	void BindButtonEvents();
	UButton* CreateStyledButton(
		const FName& ButtonName,
		const FString& Label
	);

	UFUNCTION()
	void HandleStartClicked();

	UFUNCTION()
	void HandleRestartClicked();

	UFUNCTION()
	void HandleMainMenuClicked();

	UFUNCTION()
	void HandleQuitClicked();
};
