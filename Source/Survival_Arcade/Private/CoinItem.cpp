#include "CoinItem.h"
#include "Engine/World.h"
#include "JupiterGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    // 플레이어 태그 확인
    if (Activator && Activator->ActorHasTag("Player"))
    {
		if (UWorld* World = GetWorld())
		{
			if (AJupiterGameState* GameState = World->GetGameState<AJupiterGameState>())
			{
				GameState->AddScore(PointValue);
			}
			DestroyItem();
			// 부모 클래스 (BaseItem)에 정의된 아이템 파괴 함수 호출
		}
    }
}