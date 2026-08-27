#include "JupiterPlayerController.h"
#include "EnhancedInputSubsystems.h"

AJupiterPlayerController::AJupiterPlayerController()
    : InputMappingContext(nullptr), 
      MoveAction(nullptr),
      JumpAction(nullptr),
      LookAction(nullptr),
      SprintAction(nullptr)
      // 값은 블프에서 할당할 것이므로 C++ 내에선 null처리로 해준다.
{
}

void AJupiterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
        //LocalPlayer 정보 가져오기
        //LocalPlayer는 플레이어의 입력이나 화면 뷰를 관리하는 객체
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            // UEnhancedInputLocalPlayerSubsystem를 관리하는 Subsystem 획득하기
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
            // IMC 객체 있는지 확인하기
            // InputMappingContext <-이게 존재하면 AddMappingContext라는 함수로
            // InputMappingContext 이거를 활성화 시켜 주세요.
            // 0은 가장 높은 우선순위를 의미함.(다른 IMC와 겹치는 경우 체크함)
        }
    }
}

