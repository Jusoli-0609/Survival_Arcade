#include "MineItem.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
}

void AMineItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}