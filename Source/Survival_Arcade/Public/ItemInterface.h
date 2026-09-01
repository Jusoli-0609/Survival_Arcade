// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SURVIVAL_ARCADE_API IItemInterface
{
	GENERATED_BODY()

public:
	//플레이어가 진입했을 때, 이탈했을 때 2가지의 구현이 필요하다.
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		// 오버랩이 발생한 자기 자신(스피어 컴포넌트)
		AActor* OtherActor,
		//스피어에 부딪힌 상대 액터	(캐릭터)
		UPrimitiveComponent* OtherComp,
		//OtherActor에 붙어있던, 충돌을 야기한  컴포넌트(캡슐 컴포넌트)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
		// 하위 세개는 물리 관련 변수라 나중에 다룸
	
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	//확장성을 생각했을 때 클래스보단 AActor를 쓰는 게 맞다.
	//꼭 캐릭터만 진입하는 게 아니기 때문이다. 
	virtual void ActivateItem(AActor* Activator) = 0;
	//오버랩 되면 실제 호출될 함수
	virtual FName GetItemType() const = 0;
	//아이템 유형 반환
	//보통 FString을 많이 쓰지만, 여기서 Name을 쓰는 이유는 
	//타입형을 알아낼 때는 FName이 속도, 메모리 측면에서 좋다.
	//즉, 타입을 알아내고 비교할 때는 FName을 쓴다.
};

/* 
특이점은 2개의 클래스가 생성된다는 것이다.
UINTERFACE는 리플렉션을 위한 인터페이스이므로 수정을 할 필요는 없다.
즉, IItemInterface 부터 가상함수를 쓰면 된다.
cpp 파일은 같이 생성되지만 크게 필요 없다. 그렇다고 삭제를 권하진 않는다.
빌드 시, 리플렉션 시스템이 인지할 때 헤더와 소스 파일을 쌍으로 확인하기 때문이다.
오류 가능성이 있으므로 삭제는 하지 않는 게 좋다.
또한 언리엘 코드 컨벤션에서도 헤더와 cpp는 쌍으로 존재해야한다고 쓰여있다.
-
구현으로 넘어와서, 현재의 기획 말고도 확장을 고려해 가상함수를 작성해야 한다.
* 지뢰, 힐링, 코인
힐링, 코인 - 즉시 발동형 - 오버랩 
지뢰 - 범위 내에 오버랩 - 5초 뒤 폭발 - 오버랩 - 데미지
*/