#include "MineItem.h"
#include "Components/SphereComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
    GetWorld()->GetTimerManager().SetTimer(
        ExplosionTimerHandle, 
        this, 
        &AMineItem::Explode, ExplosionDelay,
        false
    );
	// 게임 월드 -> 타이머 매니저를 통해 ExplosionTimerHandle을 설정하고, ExplosionDelay 후에 Explode 함수를 호출하도록 예약
    // 타이머 핸들러 
}

void AMineItem::Explode()
{
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 
                2.0f, FColor::Red, 
                FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));
        }
    }

    DestroyItem();
    // 지뢰 제거
}