#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase
	//데이터 테이블 행으로 쓰겠다는 의미
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	//아이템 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
	//클래스 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spawnchance;
	//라리템 확률
};
