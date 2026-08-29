#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class SURVIVAL_ARCADE_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDamage;

	virtual void ActivateItem(AActor* Activator) override;
};
