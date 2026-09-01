#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"   
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SURVIVAL_ARCADE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
    ASpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    // 스폰 영역을 담당할 박스 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem();

    FItemSpawnRow* GetRandomItem() const;
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
    // 특정 아이템 클래스를 스폰하는 함수
    FVector GetRandomPointInVolume() const;
    // 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수

};
