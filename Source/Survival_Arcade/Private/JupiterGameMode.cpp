#include "JupiterGameMode.h"
#include "JupiterCharacter.h"
#include "JupiterPlayerController.h"
#include "JupiterGameState.h"

AJupiterGameMode::AJupiterGameMode()
{
	DefaultPawnClass = AJupiterCharacter::StaticClass();
	PlayerControllerClass = AJupiterPlayerController::StaticClass();
	GameStateClass = AJupiterGameState::StaticClass();
}

//StaticClass()는 클래스 이름을 통해서 호출을 해줌...?
//클래스 반환을 해야할 때 앞으로 많이 사용한다.
// BP 내에서 해도 충분하지만 C++에서도 하면 더 안정적.