#include "BaseItem.h"
#include "Components/SphereComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	// 루트 컴포넌트 생성 및 설정

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// 충돌 컴포넌트 생성 및 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 겹침만 감지하는 프로파일 설정

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh->SetupAttachment(Collision);
		// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.
}

void ABaseItem::OnItemOverlap(AActor* OverlapActor)
{

}
void ABaseItem::OnItemEndOverlap(AActor* OverlapActor)
{

}
void ABaseItem::ActivateItem(AActor* Activator)
{

}
FName ABaseItem::GetItemType() const 
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}