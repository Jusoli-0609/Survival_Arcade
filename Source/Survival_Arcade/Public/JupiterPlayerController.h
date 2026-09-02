#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JupiterPlayerController.generated.h"

class UInputMappingContext; // IMC 관련 미리 선언
class UInputAction; // IA 관련 미리 선언

UCLASS()
class SURVIVAL_ARCADE_API AJupiterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AJupiterPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// 에디터에서 세팅할 IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	// IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	// IA_Sprint

	  // UMG 위젯 클래스를 에디터에서 할당받을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;


protected:
	virtual void BeginPlay() override;
	// IMC 활성화 함수
};
