#include "BaseItem.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

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