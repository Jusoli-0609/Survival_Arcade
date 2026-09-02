#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JupiterMenuWidget.h"
#include "JupiterPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class SURVIVAL_ARCADE_API AJupiterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AJupiterPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bShowMainMenuOnBeginPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Navigation")
	FName FirstGameLevelName = TEXT("BasicLevel");

	// 비워 두면 현재 레벨 위에 메인 메뉴를 표시한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Navigation")
	FName MainMenuLevelName = NAME_None;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHUD();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameOverMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllWidgets();

	UFUNCTION(BlueprintCallable, Category = "UI|Navigation")
	void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "UI|Navigation")
	void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "UI|Navigation")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "UI|Navigation")
	void QuitGame();

protected:
	virtual void BeginPlay() override;

private:
	void ShowMenu(
		TSubclassOf<UUserWidget> RequestedClass,
		EJupiterMenuMode MenuMode
	);

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> HUDWidget;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> ActiveMenuWidget;
};
