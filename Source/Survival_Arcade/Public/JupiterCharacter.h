#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JupiterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
// 미리 선언. 이 클래스를 가진 헤더 파일 전체를 include 하는 건 비효율적이다.
// 따라서 여기서는 이런식으로 언급만 하는 미리 선언을 이용한다.
// 구현부에 가서 include "GameFramework/SpringArmComponent.h" 과
// include "Camera/CameraComponent.h"를 해주면 된다.

UCLASS()
class SURVIVAL_ARCADE_API AJupiterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AJupiterCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") 
	// 객체 자체는 변경할 수 없지만, 내부 속성 정도는 바꾸기 가능
	//// SpringArmComp를 에디터에서 확인할 수 있고,
	// 블루프린트에서도 참조할 수 있지만 직접 다른 객체로 바꾸지는 못한다.
	// 단, SpringArmComp가 가리키는 스프링암의 내부 속성까지
	// 전부 수정 불가능해지는 것은 아니다.
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	// 체력을 회복시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	// 구조체는 무거우니 참조로 가져오고, const를 사용하는 이유는
	// 참조된 객체를 수정하지 못하도록 하기 위함임.
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	// bool 타입으로 가져왔던 건 이렇게 입력 받을 때와 아닐 때 2개로 나누어 준다.
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	// 현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
// 또는 AActor의 TakeDamage()를 오버라이드
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, 
		AActor* DamageCauser) override;

private:
	float NormalSpeed;
	// 지금 정해져 있는, 캐릭터 클래스가 가지고 있는 기본값.
	float SprintSpeedMultiplier;
	// 지금 기준에 몇 배를 곱해줄 건지.
	float SprintSpeed;
	// 위에 두 개를 곱해준 값. 얼마나 빨라졌는지를 의미.
};
