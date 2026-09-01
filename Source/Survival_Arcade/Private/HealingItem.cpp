#include "HealingItem.h"
#include "JupiterCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
	// 플레이어 캐릭터의 체력을 20만큼 회복시키는 로직 등을 구현 가능
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag(FName("Player")))
    //문자열 리터럴 "Player"를 그대로 넘기기보다 FName 형태로 명확하게 전달하는 것이 안전
    {
        if (AJupiterCharacter* PlayerCharacter = Cast<AJupiterCharacter>(Activator))
        {
            // 캐릭터의 체력을 회복
            PlayerCharacter->AddHealth(HealAmount);
        }
        /*
        GEngine->AddOnScreenDebugMessage(
            -1,
            2.0f,
            FColor::Green,
            FString::Printf(TEXT("Player gained %d HP!"), HealAmount));
        회복 디버그 메시지
        */

        DestroyItem();
    }
}