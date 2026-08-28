#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	PointValue = 50;
	ItemType = "BigCoinItem";
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}