#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class SURVIVAL_ARCADE_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor)override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;
	//인터페이스에 있던 것들.

	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	//아이템 이름 같은 경우는 기획자분들이 바꾸는경우가 많기 때문에
	//뤤만하면 리픅렉션해서 수정 가능하게 만든다.

};
