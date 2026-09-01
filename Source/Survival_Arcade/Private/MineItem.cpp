#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
						// 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함
	          UGameplayStatics::ApplyDamage(
	              Actor,                      // 데미지를 받을 액터
	              ExplosionDamage,            // 데미지 양
	              nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
	              this,                       // 데미지를 유발한 오브젝트(지뢰)
	              UDamageType::StaticClass()  // 기본 데미지 유형
	          );
				}
    }

    DestroyItem();
    // 지뢰 제거
}